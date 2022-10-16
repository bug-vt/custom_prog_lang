#ifndef _INSTRUCTION_TABLE
#define _INSTRUCTION_TABLE

#include <unordered_map>
#include <string>
#include <vector>


#define OP_FLAG_TYPE_INT    1
#define OP_FLAG_TYPE_FLOAT  2 
#define OP_FLAG_TYPE_STR    4 
#define OP_FLAG_TYPE_MEM    8
#define OP_FLAG_TYPE_LABEL  16
#define OP_FLAG_TYPE_FUNC   32
#define OP_FLAG_TYPE_REG    64

#define DESTINATION         (OP_FLAG_TYPE_MEM | OP_FLAG_TYPE_REG)
#define SOURCE              (OP_FLAG_TYPE_INT \
                             | OP_FLAG_TYPE_FLOAT \
                             | OP_FLAG_TYPE_STR   \
                             | OP_FLAG_TYPE_MEM   \
                             | OP_FLAG_TYPE_REG)
#define OP_NUMBER           (OP_FLAG_TYPE_INT \
                             | OP_FLAG_TYPE_FLOAT \
                             | OP_FLAG_TYPE_MEM   \
                             | OP_FLAG_TYPE_REG)
#define OP_WORD             (OP_FLAG_TYPE_INT \
                             | OP_FLAG_TYPE_MEM   \
                             | OP_FLAG_TYPE_REG)
#define OP_ADDR             (OP_FLAG_TYPE_INT \
                             | OP_FLAG_TYPE_MEM   \
                             | OP_FLAG_TYPE_REG)
#define OP_OFFSET           (OP_FLAG_TYPE_INT \
                             | OP_FLAG_TYPE_MEM   \
                             | OP_FLAG_TYPE_REG)
#define OP_STRING           (OP_FLAG_TYPE_STR \
                             | OP_FLAG_TYPE_MEM   \
                             | OP_FLAG_TYPE_REG)

typedef int OpBitFlags;

struct InstrLookup
{
  int opcode;
  std::vector<OpBitFlags> op_list;

  InstrLookup () { }
  InstrLookup (int opcode, std::vector<OpBitFlags> op_list);
};


class InstrLookupTable
{
  public:
    // using singleton pattern to ensure the class has one instance
    // and provide global access
    static InstrLookupTable& instance ();
    InstrLookup lookup (std::string mnemonic);
    bool isInstr (std::string mnemonic);

  private:
    std::unordered_map<std::string, InstrLookup> instr_lookup;

    InstrLookupTable ();
};

#endif
