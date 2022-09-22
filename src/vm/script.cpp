#include "script.hpp"

using std::vector;
using std::string;
using std::ifstream;

void Script::loadScript (string file_name)
{
  ifstream binary (file_name);
  // load header
  loadHeader (binary);
  
  // load instruction stream
  loadInstrStream (binary);

  // load string table
  loadStringTable (binary);

  // load function table
  loadFuncTable (binary);

 
  binary.close ();
}

void Script::loadHeader (ifstream &binary)
{
  char id[4];
  id[3] = '\0';
  binary.read ((char *) &id, 3);
  if (string (id) != "BUG")
  {
    // load error
  }
  char ver_major, ver_minor;
  binary.read (&ver_major, sizeof (char));
  binary.read (&ver_minor, sizeof (char));
  if (ver_major != 1 || ver_minor != 0)
  {
    // load error
  }
  // read stack size
  binary.read ((char *) &stack.size, sizeof (int));
  // read global data size
  binary.read ((char *) &global_data_size, sizeof (int));
  // read main func info
  binary.read (&is_main_func_present, sizeof (char));
  binary.read ((char *) &main_func_index, sizeof (int));
}

void Script::loadInstrStream (ifstream &binary)
{
  int instr_stream_size;
  binary.read ((char *) &instr_stream_size, sizeof (int));
  instr_stream = vector<Instr> (instr_stream_size);

  for (int instr_index = 0; instr_index < instr_stream_size; instr_index++)
  {
    // read opcode and operand count
    char opcode, op_count;
    binary.read (&opcode, sizeof (char));
    binary.read (&op_count, sizeof (char));
   
    instr_stream[instr_index].opcode = (int) opcode;

    // read list of operands that are used in this instruction
    for (int op_index = 0; op_index < op_count; op_index++)
    {
      Value operand;
      binary.read ((char *) &operand, sizeof (Value));
      instr_stream[instr_index].op_list.push_back (operand);
    }
  }
}

void Script::loadStringTable (ifstream &binary)
{
  // read size of string table 
  int size;
  binary.read ((char *) &size, sizeof (int));
  str_table = vector<string> (size);

  // read each string
  for (int i = 0; i < size; i++)
  {
    // read string length 
    int len;
    binary.read ((char *) &len, sizeof (int));
   
    char str[len + 1]; 
    str[len] = '\0';
    binary.read (str, len);
    
    str_table[i] = string (str);
  }
}

void Script::loadFuncTable (ifstream &binary)
{
  // read size of function table 
  int size;
  binary.read ((char *) &size, sizeof (int));
  func_table = vector<Func> (size);

  // read each function 
  for (int i = 0; i < size; i++)
  {
    int entry_point;
    int param_count;
    int local_data_size;
    binary.read ((char *) &entry_point, sizeof (int));
    binary.read ((char *) &param_count, sizeof (int));
    binary.read ((char *) &local_data_size, sizeof (int));

    func_table[i].entry_point = entry_point;
    func_table[i].param_count = param_count;
    func_table[i].local_data_size = local_data_size;
    func_table[i].stack_frame_size = param_count + local_data_size + 1;
  }
}
