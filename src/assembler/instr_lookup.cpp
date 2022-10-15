#include "instr_lookup.hpp"
#include <cassert>


using std::string;
using std::unordered_map;
using std::vector;


InstrLookup::InstrLookup (int opcode, vector<OpBitFlags> op_list)
{
  this->opcode = opcode;
  this->op_list = vector<OpBitFlags>(op_list);
}

InstrLookupTable& InstrLookupTable::instance ()
{
  static InstrLookupTable *instance = new InstrLookupTable ();
  return *instance;
}

InstrLookupTable::InstrLookupTable ()
{
  // memory
  instr_lookup["mov"] = {0, {DESTINATION, SOURCE}};
  instr_lookup["mem"] = {1, {DESTINATION, SOURCE, SRC_INTEGER}};
  instr_lookup["ref"] = {2, {DESTINATION, OP_FLAG_TYPE_MEM}}; 
  // arithmetic
  instr_lookup["add"] = {3, {DESTINATION, SRC_NUMBER}};
  instr_lookup["sub"] = {4, {DESTINATION, SRC_NUMBER}};
  instr_lookup["mul"] = {5, {DESTINATION, SRC_NUMBER}};
  instr_lookup["div"] = {6, {DESTINATION, SRC_NUMBER}};
  instr_lookup["mod"] = {7, {DESTINATION, SRC_NUMBER}};
  instr_lookup["exp"] = {8, {DESTINATION, SRC_NUMBER}};
  instr_lookup["neg"] = {9, {DESTINATION}};
  instr_lookup["inc"] = {10, {DESTINATION}};
  instr_lookup["dec"] = {11, {DESTINATION}};
  // bitwise
  instr_lookup["and"] = {12, {DESTINATION, SRC_INTEGER}};
  instr_lookup["or"] =  {13, {DESTINATION, SRC_INTEGER}};
  instr_lookup["xor"] = {14, {DESTINATION, SRC_INTEGER}};
  instr_lookup["not"] = {15, {DESTINATION}};
  instr_lookup["shl"] = {16, {DESTINATION, SRC_INTEGER}};
  instr_lookup["shr"] = {17, {DESTINATION, SRC_INTEGER}};
  // string processing
  instr_lookup["concat"] =  {18, {DESTINATION, SRC_STRING}};
  instr_lookup["getChar"] = {19, {DESTINATION, SRC_STRING, SRC_INTEGER}};
  instr_lookup["setChar"] = {20, {DESTINATION, SRC_INTEGER, SRC_STRING}};
  // conditional branching
  instr_lookup["jmp"] = {21, {OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["je"] =  {22, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["jne"] = {23, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["jg"] =  {24, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["jl"] =  {25, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["jge"] = {26, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["jle"] = {27, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  // stack interface
  instr_lookup["push"] = {28, {SOURCE}}; 
  instr_lookup["pop"]  = {29, {DESTINATION}}; 
  // function interface
  instr_lookup["call"] = {30, {OP_FLAG_TYPE_FUNC}}; 
  instr_lookup["ret"] =  {31, {}}; 
  // miscellaneous
  instr_lookup["pause"] = {32, {OP_FLAG_TYPE_INT}};
  instr_lookup["exit"]  = {33, {OP_FLAG_TYPE_INT}};
  instr_lookup["print"]  = {34, {SOURCE}};
}

InstrLookup InstrLookupTable::lookup (string mnemonic)
{
  return instr_lookup[mnemonic];
}

bool InstrLookupTable::isInstr (string mnemonic)
{
  if (instr_lookup.find (mnemonic) == instr_lookup.end ())
    return false;

  return true;
}
