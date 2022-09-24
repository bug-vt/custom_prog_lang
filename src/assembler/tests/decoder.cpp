#include "decoder.hpp"

using std::string;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::stringstream;

//#define MAIN 1


#ifdef MAIN
int main (int argc, char **argv)
{
  if (argc < 2)
  {
    cout << "usage: decoder executable_file <--short>" << endl;
    exit (1);
  }

  bool show_less = false;
  if (argc > 2)
  {
    if (string (argv[2]) == "--short")
      show_less = true;
  }

  string out = "";
  ifstream binary (argv[1]);

  out = Decoder::readHeader (binary);
  if (!show_less)
    cout << out << endl;

  cout << Decoder::readInstrStream (binary) << endl;

  out = Decoder::readStringTable (binary);
  if (!show_less)
    cout << out << endl;

  out = Decoder::readFuncTable (binary);
  if (!show_less)
    cout << out << endl;

  binary.close ();

  return 0;
}
#endif


string Decoder::readHeader (ifstream &binary)
{
  // read header
  TestHeader header;
  stringstream out;

  char id[4];
  id[3] = '\0';
  binary.read ((char *) &id, 3);
  out << "Header:" << endl
      << string (id) << endl;


  binary.read (&header.ver_major, sizeof (char));
  binary.read (&header.ver_minor, sizeof (char));
  binary.read ((char *) &header.stack_size, sizeof (int));
  binary.read ((char *) &header.global_data_size, sizeof (int));
  binary.read (&header.is_main_func_present, sizeof (char));
  binary.read ((char *) &header.main_func_index, sizeof (int));

  out << (int) header.ver_major << " "
      << (int) header.ver_minor << endl
      << header.stack_size << " "
      << header.global_data_size << " "
      << (bool) header.is_main_func_present << " "
      << header.main_func_index << endl;

  return out.str ();
}

string Decoder::readInstrStream (ifstream &binary)
{
  stringstream out;
  // read size of instruction stream
  int size;
  binary.read ((char *) &size, sizeof (int));

  out << "Instruction stream:" << endl
      << size << endl;

  // read each instruction
  for (int i = 0; i < size; i++)
  {
    // read opcode and operand count
    char opcode, op_count;
    binary.read (&opcode, sizeof (char));
    binary.read (&op_count, sizeof (char));
   
    out << (int) opcode << " "
        << (int) op_count;
    if (op_count > 0)
      out << " ";
    else
      out << endl;

    // read list of operands that are used in this instruction
    for (int op_index = 0; op_index < op_count; op_index++)
    {
      TestOp op;
      binary.read ((char *) &op, sizeof (TestOp));

      // operand type 
      out << op.type << " ";
      // operand value
      if (op.type == OP_TYPE_FLOAT)
        out << op.float_literal << " ";
      else
        out << op.int_literal << " ";

      // offset (for relative indexing)
      out << op.offset_index;
       
      // last operand print newline
      if (op_index < op_count - 1)
        out << " ";
      else
        out << endl;
    }
  }

  return out.str ();
}

string Decoder::readStringTable (ifstream &binary)
{
  stringstream out;
  // read size of string table 
  int size;
  binary.read ((char *) &size, sizeof (int));

  out << "String table:" << endl
      << size << endl;

  // read each string
  for (int i = 0; i < size; i++)
  {
    // read string length 
    int len;
    binary.read ((char *) &len, sizeof (int));
   
    char str[len + 1]; 
    str[len] = '\0';
    binary.read (str, len);

    out << len << " "
        << string (str) << endl;
  }

  return out.str ();
}

string Decoder::readFuncTable (ifstream &binary)
{
  stringstream out;
  // read size of function table 
  int size;
  binary.read ((char *) &size, sizeof (int));

  out << "Function table:" << endl
      << size << endl;

  // read each function 
  for (int i = 0; i < size; i++)
  {
    int entry_point;
    int param_count;
    int local_data_size;
    binary.read ((char *) &entry_point, sizeof (int));
    binary.read ((char *) &param_count, sizeof (int));
    binary.read ((char *) &local_data_size, sizeof (int));

    out << entry_point << " "
        << param_count << " "
        << local_data_size << endl;
  }

  return out.str ();
}
