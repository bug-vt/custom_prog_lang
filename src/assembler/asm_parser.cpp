#include "asm_parser.hpp"
#include "error.hpp"

#define GLOBAL_SCOPE 0
#define DEFAULT_STACK_SIZE 1024
#define FIRST_PASS 1
#define SECOND_PASS 2

using std::string;

AsmParser::AsmParser (string raw_source)
{
  lexer = AsmLexer (raw_source);

  stack_size = DEFAULT_STACK_SIZE; 
  is_main_func_present = false;
  main_func_index = -1;
  global_data_size = 0;
}

CodeGen AsmParser::createCodeGen (std::ostream &out_file)
{
  CodeGen code_gen (out_file);
  // put header
  code_gen.header.stack_size = stack_size;
  code_gen.header.is_main_func_present = (char) is_main_func_present;
  code_gen.header.main_func_index = main_func_index;
  code_gen.header.global_data_size = global_data_size;
  // put tables 
  code_gen.sym_table = symbol_table;
  code_gen.label_table = label_table;
  code_gen.func_table = func_table;
  code_gen.str_table = str_table;
  // put instruction stream
  code_gen.instr_stream = instr_stream;

  return code_gen;
}

void AsmParser::readToken (Token req_token)
{
  if (lexer.getNextToken () != req_token)
  {
    string msg = token2string (req_token) + " expected";
    exitOnCodeError (msg, lexer); 
  }
}

void AsmParser::parse ()
{
  parse (FIRST_PASS);
  lexer.reset ();
  parse (SECOND_PASS);
}

void AsmParser::parse (int pass_id)
{
  instr_count = 0;
  curr_pass = pass_id;
  curr_scope = GLOBAL_SCOPE; 

  while (true)
  {
    if (lexer.peekNextToken () == TOKEN_TYPE_EOF)
      return;
    else
      parseLine ();
  }
}

void AsmParser::parseLine ()
{
  Token first_token = lexer.getNextToken ();
  switch (first_token)
  {
    case TOKEN_TYPE_NEWLINE:
      return;

    case TOKEN_TYPE_FUNC:
      parseFunc ();
      break;

    case TOKEN_TYPE_VAR:
      if (curr_pass == FIRST_PASS)
        parseVar ();
      else
        lexer.goToNextLine ();
      break;

    case TOKEN_TYPE_PARAM:
      parseParam ();
      break;

    case TOKEN_TYPE_IDENT:
      if (curr_pass == FIRST_PASS)
        parseLabel ();
      else
        lexer.goToNextLine ();
      break;

    case TOKEN_TYPE_INSTR:
      if (curr_pass == SECOND_PASS)
        parseInstr ();
      else
      {
        lexer.goToNextLine ();
        instr_count++;
      }
      break;

    default:
      exitOnCodeError ("Unexpected token", lexer);
  }
}


void AsmParser::parseFunc ()
{
  if (curr_scope != GLOBAL_SCOPE)
    exitOnCodeError ("Function cannot be defined inside another function", lexer);

  // function name
  readToken (TOKEN_TYPE_IDENT);
  string func_name = lexer.getCurrLexeme ();
  int func_index = -1;

  if (curr_pass == FIRST_PASS)
  {
    // instruction immediately after the current one is the function's entry point
    // which is equal to the current instruction count
    int entry_point = instr_count;
    // add function to the function table and check for redefinition
    func_index = func_table.addFunc (func_name, entry_point); 
    if (func_index == -1)
      exitOnCodeError ("Function was already defined somewhere else", lexer);

    if (func_name == "main")
    {
      is_main_func_present = true;
      main_func_index = func_index; 
    }
  }
  else
    func_index = func_table.getFuncIndex (func_name); 

  // change scope to the function
  // and reinitialize variables that used for tracking function
  curr_scope = func_index;
  curr_func_param_size = 0;
  curr_func_local_data_size = 0;

  while (lexer.peekNextToken () == TOKEN_TYPE_NEWLINE)
    readToken (TOKEN_TYPE_NEWLINE);

  // start the code block with opening curly brace.
  readToken (TOKEN_TYPE_OPEN_BRACE);

  // parse function body
  parseBlock ();

  if (curr_pass == SECOND_PASS)
  {
    // add ret instruction at the end of the function.
    Instr ret_instr (instr_table.lookup ("ret").opcode, 0);
    instr_stream.push_back (ret_instr);
  }
  else
  {
    // ret instruction will be added at the end of the function during the
    // second pass, so count additional instruction
    instr_count++;
    // finish setting up function
    func_table.setFunc (curr_scope, curr_func_param_size, curr_func_local_data_size);
  }

  // once block have been parsed, return to global scope
  curr_scope = GLOBAL_SCOPE;
}


void AsmParser::parseBlock ()
{
  if (curr_scope == GLOBAL_SCOPE)
    exitOnCodeError ("Code block can only be used for defining function scope", lexer);

  while (lexer.peekNextToken () != TOKEN_TYPE_CLOSE_BRACE)
    parseLine ();

  // end the code block with closing curly brace.
  readToken (TOKEN_TYPE_CLOSE_BRACE);
  readToken (TOKEN_TYPE_NEWLINE);
}


void AsmParser::parseVar ()
{
  readToken (TOKEN_TYPE_IDENT);

  string ident = lexer.getCurrLexeme ();
  // for now, consider as a variable (array have size >= 1)
  int size = 1;

  // if next token is open bracket,
  // verify if the identifier is an array and change to identified size
  if (lexer.peekNextToken () == TOKEN_TYPE_OPEN_BRACKET)
  {
    readToken (TOKEN_TYPE_OPEN_BRACKET);

    readToken (TOKEN_TYPE_INT);
    size = stoi (lexer.getCurrLexeme ());
    
    readToken (TOKEN_TYPE_CLOSE_BRACKET);
  }

  // calculate variable's index inside stack
  // global variables reside on the bottom of the stack 
  // and indexed by positive stack index.
  int stack_index;
  if (curr_scope == GLOBAL_SCOPE)
    stack_index = global_data_size;
  // whereas local variables reside on the top of the stack
  // and indexed by negative stack index.
  // The top of the stack index is -1,
  // but the top of the stack is reserved for VM.
  // So, offset 1 must be added.
  else
    stack_index = -1 - (1 + curr_func_local_data_size);

  // add symbol to the symbol table and check for redefinition
  Symbol symbol (ident, curr_scope); 
  if (symbol_table.addSymbol (symbol, size, stack_index) == -1)
    exitOnCodeError ("Identifier with same name already exists inside the same scope", lexer);

  // grow the global or local data size
  if (curr_scope == GLOBAL_SCOPE)
    global_data_size += size;
  else
    curr_func_local_data_size += size;
  
  readToken (TOKEN_TYPE_NEWLINE);
}


void AsmParser::parseLabel ()
{
  string ident = lexer.getCurrLexeme ();

  readToken (TOKEN_TYPE_COLON);

  if (curr_scope == GLOBAL_SCOPE)
    exitOnCodeError ("Line label can be only used inside the function", lexer);

  int target_index = instr_stream.size ();
  int func_index = curr_scope;

  Label label (ident, func_index);
  if (label_table.addLabel (label, target_index) == -1)
    exitOnCodeError ("Label with same name already exists inside the same scope", lexer);
}

// parse at 2nd pass
void AsmParser::parseParam ()
{
  if (curr_scope == GLOBAL_SCOPE)
    exitOnCodeError ("parameter cannot be defined on global scope", lexer);

  readToken (TOKEN_TYPE_IDENT);

  if (curr_pass == SECOND_PASS)
  {
    string ident = lexer.getCurrLexeme ();

    // determine stack index.
    // params are pushed before the function call, so reside below the 
    // local data and return address on stack.
    // Since top of the stack index is -1, param is located at
    // = -1 - 1(reserved for VM) - local data size - 1(return address) - param index 
    FuncInfo curr_func = func_table.at (curr_scope);
    int stack_index = -1 - (1 + curr_func.local_data_size + 1 + curr_func_param_size);

    Symbol symbol (ident, curr_scope); 
    if (symbol_table.addSymbol (symbol, 1, stack_index) == -1)
      exitOnCodeError ("Identifier with same name already exists inside the same scope", lexer);
  }

  curr_func_param_size++;

  readToken (TOKEN_TYPE_NEWLINE);
}


// parse at 2nd pass
void AsmParser::parseInstr ()
{
  // lookup the reference instruction from the given mnemonic
  string mnemonic = lexer.getCurrLexeme ();
  InstrLookup ref_instr = instr_table.lookup (mnemonic);

  // Start forming the output instruction
  int op_count = ref_instr.op_list.size ();
  Instr curr_output (ref_instr.opcode, op_count);

  // process and verify each operand
  for (int op_index = 0; op_index < op_count; op_index++)
  {
    // get bit encoding that represent allowed operand types
    // for current operand 
    OpBitFlags op_flags = ref_instr.op_list[op_index];
   
    // token type for current operand
    Token op_token = lexer.getNextToken ();

    // verify operand type match with one of the bit flags
    OpBitFlags curr_op_type = token2bitflag (op_token);
    if (!(op_flags & curr_op_type))
      exitOnCodeError ("Invalid operand for given instruction", lexer);

    // record type and value
    // which will be used for generating binary
    OpType op_type = token2op (op_token, op_flags);
    curr_output.op_list[op_index].type = op_type;

    string curr_lexeme = lexer.getCurrLexeme ();
    switch (op_type)
    {
      case OP_TYPE_INT:
        curr_output.op_list[op_index].int_literal = stoi (curr_lexeme);
        break;
      case OP_TYPE_FLOAT:
        curr_output.op_list[op_index].float_literal = stof (curr_lexeme);
        break;
      case OP_TYPE_STR:
        {
          int str_table_index = str_table.addString (curr_lexeme);
          curr_output.op_list[op_index].str_table_index = str_table_index;
        }
        break;
      case OP_TYPE_INSTR_INDEX:
        {
          Label label (curr_lexeme, curr_scope);
          int target_index = label_table.getTargetIndex (label);
          if (target_index == -1)
            exitOnCodeError ("Undefined label", lexer);
          curr_output.op_list[op_index].instr_index = target_index;
        }
        break;
      case OP_TYPE_ABS_STACK_INDEX:
        {
          Symbol symbol (curr_lexeme, curr_scope);
          int stack_index = symbol_table.getStackIndex (symbol);
          if (stack_index == -1)
            exitOnCodeError ("Undefined identifier", lexer);
          // if next token is open bracket,
          // verify if the operand is an array and process it 
          if (lexer.peekNextToken () == TOKEN_TYPE_OPEN_BRACKET)
          {
            readToken (TOKEN_TYPE_OPEN_BRACKET);
          
            Token index_token = lexer.getNextToken ();
            // for integer index, then add add offset to the base stack index
            if (index_token == TOKEN_TYPE_INT)
              stack_index += stoi (lexer.getCurrLexeme ());
            // for variable index, record index of the variable and change
            // operand type to relative indexing
            // because we won't know the value of variable until runtime
            else if (index_token == TOKEN_TYPE_IDENT)
            {
              curr_output.op_list[op_index].type = OP_TYPE_REL_STACK_INDEX;
              // find the stack index of the variable index
              string ident = lexer.getCurrLexeme ();
              Symbol offset_symbol (ident, curr_scope);
              int offset_index = symbol_table.getStackIndex (offset_symbol);
              if (offset_index == -1)
                exitOnCodeError ("Undefined identifier", lexer);
              // record the stack index that will be added to the base index
              // during runtime
              curr_output.op_list[op_index].offset_index = offset_index;
            }
            else
              exitOnCodeError ("Invalid token for array indexing", lexer);
            
            readToken (TOKEN_TYPE_CLOSE_BRACKET);
          }
          curr_output.op_list[op_index].stack_index = stack_index;
        }
        break;

      case OP_TYPE_FUNC:
        {
          int func_index = func_table.getFuncIndex (curr_lexeme);
          if (func_index == -1)
            exitOnCodeError ("Undefined function", lexer);
          curr_output.op_list[op_index].func_index = func_index;
        }
        break;

      case OP_TYPE_REG:
        curr_output.op_list[op_index].reg = 0;
        break;

      default:
        exitOnCodeError ("Unsupported operand type", lexer);
    }
    
    if (op_index < op_count - 1)
      readToken (TOKEN_TYPE_COMMA);
  }

  readToken (TOKEN_TYPE_NEWLINE);
  // push the processed instruction to instruction stream
  instr_stream.push_back (curr_output);
}
