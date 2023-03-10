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
  instr_lookup["ref"] = {1, {DESTINATION, OP_FLAG_TYPE_MEM}}; 
  instr_lookup["lw"]  = {2, {DESTINATION, OP_ADDR, OP_OFFSET}};
  instr_lookup["sw"]  = {3, {OP_ADDR, OP_OFFSET, SOURCE}};
  // arithmetic
  instr_lookup["add"] = {4, {DESTINATION, OP_NUMBER}};
  instr_lookup["sub"] = {5, {DESTINATION, OP_NUMBER}};
  instr_lookup["mul"] = {6, {DESTINATION, OP_NUMBER}};
  instr_lookup["div"] = {7, {DESTINATION, OP_NUMBER}};
  instr_lookup["mod"] = {8, {DESTINATION, OP_NUMBER}};
  instr_lookup["exp"] = {9, {DESTINATION, OP_NUMBER}};
  instr_lookup["neg"] = {10, {DESTINATION}};
  instr_lookup["inc"] = {11, {DESTINATION}};
  instr_lookup["dec"] = {12, {DESTINATION}};
  // bitwise
  instr_lookup["and"] = {13, {DESTINATION, OP_WORD}};
  instr_lookup["or"] =  {14, {DESTINATION, OP_WORD}};
  instr_lookup["xor"] = {15, {DESTINATION, OP_WORD}};
  instr_lookup["not"] = {16, {DESTINATION}};
  instr_lookup["shl"] = {17, {DESTINATION, OP_OFFSET}};
  instr_lookup["shr"] = {18, {DESTINATION, OP_OFFSET}};
  // string processing (+ add instruction for string concatenation)
  instr_lookup["getChar"] = {19, {DESTINATION, OP_STRING, OP_OFFSET}};
  instr_lookup["setChar"] = {20, {DESTINATION, OP_OFFSET, OP_STRING}};
  // comparison
  instr_lookup["seq"] =  {21, {DESTINATION, SOURCE, SOURCE}}; 
  instr_lookup["sne"] =  {22, {DESTINATION, SOURCE, SOURCE}}; 
  instr_lookup["sgt"] =  {23, {DESTINATION, SOURCE, SOURCE}}; 
  instr_lookup["slt"] =  {24, {DESTINATION, SOURCE, SOURCE}}; 
  instr_lookup["sge"] =  {25, {DESTINATION, SOURCE, SOURCE}}; 
  instr_lookup["sle"] =  {26, {DESTINATION, SOURCE, SOURCE}}; 
  // branching
  instr_lookup["jmp"] = {27, {OP_FLAG_TYPE_LABEL}}; 
  instr_lookup["je"] =  {28, {SOURCE, SOURCE, OP_FLAG_TYPE_LABEL}}; 
  // stack interface
  instr_lookup["push"] = {29, {SOURCE}}; 
  instr_lookup["pop"]  = {30, {DESTINATION}}; 
  // function interface
  instr_lookup["call"] = {31, {OP_FLAG_TYPE_FUNC}}; 
  instr_lookup["ret"] =  {32, {}}; 
  // miscellaneous
  instr_lookup["pause"] = {33, {OP_FLAG_TYPE_INT}};
  instr_lookup["exit"]  = {34, {}};
  instr_lookup["print"]  = {35, {SOURCE}};
  instr_lookup["time"] =  {36, {}}; 
  instr_lookup["randint"] =  {37, {}}; 
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
