#!/usr/bin/env python3

import sys

def main (argc, argv):
  if (argc != 2):
    print ("Usage: generate_ast <output directory>")
    return

  output_dir = argv[1]
  types = ["Binary   : Expr *left, Token op, Expr *right",
           "Grouping : Expr *expression",
           "Literal  : Op value",
           "Unary    : Token op, Expr *right"]

  defineAst (output_dir, "Expr", types)


def defineAst (output_dir, base_name, types):
  output = open (output_dir + "/" + base_name.lower () + ".hpp", "w")
 
  output.write ("#ifndef EXPR_HPP\n")
  output.write ("#define EXPR_HPP\n")
  output.write ("\n")
  output.write ("#include \"icode.hpp\"\n")
  output.write ("#include \"token.hpp\"\n")
  output.write ("#include <string>\n")
  output.write ("\n")
  output.write ("class Expr;\n")
  output.write ("class Binary;\n")
  output.write ("class Grouping;\n")
  output.write ("class Literal;\n")
  output.write ("class Unary;\n")
  output.write ("\n")
  defineVisitor (output, base_name, types)
  output.write ("\n")
  output.write ("struct " + base_name + "\n")
  output.write ("{\n")
  output.write ("  virtual std::string accept (Visitor &visitor) { return \"\"; }\n")
  output.write ("};\n")
  output.write ("\n")

  for elem in types:
    class_name = elem.split (":")[0].strip ()
    fields = elem.split(":")[1].strip ()
    defineType (output, base_name, class_name, fields)

  output.write ("#endif\n")

  output.close ()


def defineType (output, base_name, class_name, fields):
  output.write ("struct " + class_name + " : public " + base_name + "\n")
  output.write ("{\n")
  # constructor
  output.write ("  " + class_name + " (" + fields + ")\n")
  output.write ("  {\n")
 
  # store parameters in fields
  field_list = fields.split (", ")
  for field in field_list:
    name = field.split ()[1]
    if "*" in name:
      name = name.split ("*")[1]
    output.write ("    this->" + name + " = " + name + ";\n")

  output.write ("  }\n")

  # visitor pattern. visit method for its own type
  output.write ("\n")
  output.write ("  virtual std::string accept (Visitor &visitor)\n")
  output.write ("  {\n")
  output.write ("    return visitor.visit" + class_name + base_name + " (this);\n");
  output.write ("  }\n")

  # fields
  output.write ("\n")
  for field in field_list:
    output.write ("  " + field + ";\n")

  output.write ("};\n")
  output.write ("\n")


def defineVisitor (output, base_name, types):
  output.write ("struct Visitor\n")
  output.write ("{\n")
  for elem in types:
    type_name = elem.split (":")[0].strip ()
    output.write ("  virtual std::string visit" + type_name + base_name + " (" + \
                  type_name + " *" + base_name.lower () + ") = 0;\n")
  output.write ("};\n")


if __name__ == '__main__':
  main (len(sys.argv), sys.argv)
