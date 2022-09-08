#ifndef _INSTRUCTION
#define _INSTRUCTION

#include <vector>

enum OpType {OP_TYPE_INT,
             OP_TYPE_FLOAT,
             OP_TYPE_STR,
             OP_TYPE_MEM,
             OP_TYPE_INSTR_INDEX,
             OP_TYPE_FUNC,
             OP_TYPE_REG,
             OP_TYPE_INVALID};

struct Op
{
    OpType type;
    union
    {
        int int_literal;
        float float_literal;
        int str_table_index;    // string table index
        int instr_index;        // instruction stream index
        int stack_index;        // stack index
        int func_index;         // function table index
        int reg;                // register code
    };
    int offset_index; 
};

struct Instr
{
    int opcode;
    std::vector<Op> op_list; 

    Instr () { }
    Instr (int opcode, int op_count) 
    { 
      this->opcode = opcode; 
      op_list = std::vector<Op> (op_count);
    }
};

#endif
