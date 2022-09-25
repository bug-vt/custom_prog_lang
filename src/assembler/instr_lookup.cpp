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
  // arithmetic
  instr_lookup["add"] = {1, {DESTINATION, SOURCE}};
  instr_lookup["sub"] = {2, {DESTINATION, SOURCE}};
  instr_lookup["mul"] = {3, {DESTINATION, SOURCE}};
  instr_lookup["div"] = {4, {DESTINATION, SOURCE}};
  instr_lookup["mod"] = {5, {DESTINATION, SOURCE}};
  instr_lookup["exp"] = {6, {DESTINATION, SOURCE}};
  instr_lookup["neg"] = {7, {DESTINATION}};
  instr_lookup["inc"] = {8, {DESTINATION}};
  instr_lookup["dec"] = {9, {DESTINATION}};
  // bitwise
  instr_lookup["and"] = {10, {DESTINATION, SOURCE}};
  instr_lookup["or"] =  {11, {DESTINATION, SOURCE}};
  instr_lookup["xor"] = {12, {DESTINATION, SOURCE}};
  instr_lookup["not"] = {13, {DESTINATION}};
  instr_lookup["shl"] = {14, {DESTINATION, SHIFT_COUNT}};
  instr_lookup["shr"] = {15, {DESTINATION, SHIFT_COUNT}};
  // string processing
  instr_lookup["concat"] =  {16, {OP_FLAG_TYPE_STR, OP_FLAG_TYPE_STR}};
  instr_lookup["getChar"] = {17, {DESTINATION, SOURCE, OP_FLAG_TYPE_INT}};
  instr_lookup["setChar"] = {18, {OP_FLAG_TYPE_INT, DESTINATION, SOURCE}};
  // conditional branching
  instr_lookup["jmp"] = {19, {OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["je"] =  {20, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["jne"] = {21, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["jg"] =  {22, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["jl"] =  {23, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["jge"] = {24, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["jle"] = {25, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  // stack interface
  instr_lookup["push"] = {26, {SOURCE}}; 
  instr_lookup["pop"]  = {27, {DESTINATION}}; 
  // function interface
  instr_lookup["call"] = {28, {OP_FLAG_TYPE_FUNC}}; 
  instr_lookup["ret"] =  {29, {}}; 
  // miscellaneous
  instr_lookup["pause"] = {30, {OP_FLAG_TYPE_INT}};
  instr_lookup["exit"]  = {31, {OP_FLAG_TYPE_INT}};
  instr_lookup["print"]  = {32, {SOURCE}};
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
