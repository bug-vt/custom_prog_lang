#include "token.hpp"

using std::string;

string token2string (Token token)
{
  string out;
  switch (token)
  {
    case TOKEN_TYPE_EOF:
      out = "EOF";
      break;
    case TOKEN_TYPE_INT:
      out = "Int";
      break;
    case TOKEN_TYPE_FLOAT:
      out = "Float";
      break;
    case TOKEN_TYPE_STRING:
      out = "String";
      break;
    case TOKEN_TYPE_IDENT:
      out = "Identifier";
      break;
    case TOKEN_TYPE_COLON:
      out = "Colon";
      break;
    case TOKEN_TYPE_OPEN_BRACKET:
      out = "Open bracket";
      break;
    case TOKEN_TYPE_CLOSE_BRACKET:
      out = "Close bracket";
      break;
    case TOKEN_TYPE_COMMA:
      out = "Comma";
      break;
    case TOKEN_TYPE_OPEN_BRACE:
      out = "Open brace";
      break;
    case TOKEN_TYPE_CLOSE_BRACE:
      out = "Close brace";
      break;
    case TOKEN_TYPE_NEWLINE:
      out = "New line";
      break;
    case TOKEN_TYPE_INSTR:
      out = "Instruction";
      break;
    case TOKEN_TYPE_SETSTACKSIZE:
      out = "setStackSize";
      break;
    case TOKEN_TYPE_VAR:
      out = "var";
      break;
    case TOKEN_TYPE_FUNC:
      out = "func";
      break;
    case TOKEN_TYPE_PARAM:
      out = "param";
      break;
    case TOKEN_TYPE_REG_RETVAL:
      out = "_retVal";
      break;
    default:
      out = "Invalid";
  }
  return out;
}


OpBitFlags token2bitflag (Token token)
{
  OpBitFlags op_flag;
  switch (token)
  {
    case TOKEN_TYPE_INT:
      op_flag = OP_FLAG_TYPE_INT;
      break;
    case TOKEN_TYPE_FLOAT:
      op_flag = OP_FLAG_TYPE_FLOAT;
      break;
    case TOKEN_TYPE_STRING:
      op_flag = OP_FLAG_TYPE_STR;
      break;
    case TOKEN_TYPE_IDENT:
      op_flag = OP_FLAG_TYPE_MEM;
      break;
    case TOKEN_TYPE_FUNC:
      op_flag = OP_FLAG_TYPE_FUNC;
      break;
    case TOKEN_TYPE_REG_RETVAL:
      op_flag = OP_FLAG_TYPE_REG;
      break;
    default:
      op_flag = 0;
  }
  return op_flag;
}

OpType token2op (Token token)
{
  OpType op;
  switch (token)
  {
    case TOKEN_TYPE_INT:
      op = OP_TYPE_INT;
      break;
    case TOKEN_TYPE_FLOAT:
      op = OP_TYPE_FLOAT;
      break;
    case TOKEN_TYPE_STRING:
      op = OP_TYPE_STR;
      break;
    case TOKEN_TYPE_IDENT:
      op = OP_TYPE_MEM;
      break;
    case TOKEN_TYPE_FUNC:
      op = OP_TYPE_FUNC;
      break;
    case TOKEN_TYPE_REG_RETVAL:
      op = OP_TYPE_REG;
      break;
    default:
      op = OP_TYPE_INVALID;
  }
  return op;
}
