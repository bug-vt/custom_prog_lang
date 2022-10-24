#ifndef ICODE_HPP
#define ICODE_HPP

#include <string>
#include <vector>


enum InstrOpcode {INSTR_MOV,      // 0
                  INSTR_REF,      // 1
                  INSTR_LW,       // 2
                  INSTR_SW,       // 3
                  INSTR_ADD,      // 4 
                  INSTR_SUB,      // 5 
                  INSTR_MUL,      // 6 
                  INSTR_DIV,      // 7 
                  INSTR_MOD,      // 8 
                  INSTR_EXP,      // 9 
                  INSTR_NEG,      // 10
                  INSTR_INC,      // 11 
                  INSTR_DEC,      // 12
                  INSTR_AND,      // 13
                  INSTR_OR,       // 14
                  INSTR_XOR,      // 15
                  INSTR_NOT,      // 16
                  INSTR_SHL,      // 17
                  INSTR_SHR,      // 18
                  INSTR_CONCAT,   // 19
                  INSTR_GETCHAR,  // 20
                  INSTR_SETCHAR,  // 21
                  INSTR_JMP,      // 22
                  INSTR_JE,       // 23
                  INSTR_JNE,      // 24
                  INSTR_JG,       // 25
                  INSTR_JL,       // 26
                  INSTR_JGE,      // 27
                  INSTR_JLE,      // 28
                  INSTR_PUSH,     // 29
                  INSTR_POP,      // 30 
                  INSTR_CALL,     // 31
                  INSTR_RET,      // 32
                  INSTR_PAUSE,    // 33
                  INSTR_EXIT,     // 34
                  INSTR_PRINT};   // 35



enum OpType {OP_TYPE_INT,
             OP_TYPE_FLOAT,
             OP_TYPE_STR_INDEX,
             OP_TYPE_VAR,
             OP_TYPE_ABS_ARRAY_INDEX,
             OP_TYPE_VAR_ARRAY_INDEX,
             OP_TYPE_JUMP_TARGET_INDEX,
             OP_TYPE_FUNC_INDEX,
             OP_TYPE_REG,
             OP_TYPE_INVALID};


enum ICodeNodeType {ICODE_NODE_INSTR, 
                    ICODE_NODE_SOURCE_LINE,
                    ICODE_NOTE_JUMP_TARGET};

struct Op 
{
  OpType type;
  union
  {
    int int_literal;
    float float_literal;
    int string_index;
    int symbol_index;
    int jump_target_index;
    int func_index;
    int reg;
  };

  int offset;               // constant offset
  int offset_symbol_index;  // variable offset
};

struct ICodeInstr
{
  int opcode;
  std::vector<Op> op_list;
};

struct ICodeNode
{
  int type;

  ICodeInstr instr;
  std::string source_line;
  int jump_target_index;
};

class ICode
{
  public:
    ICode (); 
    int addInstr (int opcode);
    void addOp (int instr_index, Op value);
    void addIntOp (int instr_index, int int_literal);
    void addVarOp (int instr_index, int symbol_index); 

  private:
    int icode_count;
    std::vector<ICodeNode> icode_stream;

  friend class CodeGen;
};

#endif
