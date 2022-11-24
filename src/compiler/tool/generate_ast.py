#!/usr/bin/env python3

import sys

def main (argc, argv):
  if (argc != 2):
    print ("Usage: generate_ast <output directory>")
    return

  output_dir = argv[1]
  expr_types = ["Assign   = Token name, Expr *value, int scope",
                "Binary   = Expr *left, Token op, Expr *right",
                "Grouping = Expr *expression",
                "Literal  = Token value",
                "Unary    = Token op, Expr *right",
                "Variable = Token name, int scope"]

  defineAst (output_dir, "Expr", expr_types)

  stmt_types = ["Block      = std::vector<Stmt*> statements",
                "Expression = Expr *expression",
                "If         = Expr *condition, Stmt *thenBranch, Stmt *elseBranch",
                "Print      = Expr *expression",
                "Var        = Token name, Expr *initializer, int scope"]

  defineAst (output_dir, "Stmt", stmt_types)


def defineAst (output_dir, base_name, types):
  output = open (output_dir + "/" + base_name.lower () + ".hpp", "w")
 
  output.write ("#ifndef %s_HPP\n" % base_name.upper ())
  output.write ("#define %s_HPP\n" % base_name.upper ())
  output.write ("\n")
  output.write ("#include \"token.hpp\"\n")
  output.write ("#include \"expr.hpp\"\n")
  output.write ("#include <string>\n")
  output.write ("#include <vector>\n")
  output.write ("\n")

  # forward referencing
  for elem in types:
    class_name = elem.split ("=")[0].strip ()
    output.write ("struct " + class_name + ";\n")

  output.write ("\n")
  defineVisitor (output, base_name, types)
  output.write ("\n")
  output.write ("struct " + base_name + "\n")
  output.write ("{\n")
  output.write ("  virtual std::string accept (%sVisitor &visitor)\n" % base_name)
  output.write ("  {\n")
  output.write ("    throw std::runtime_error (\"Visiting Expr base class\");\n")
  output.write ("  }\n")
  output.write ("};\n")
  output.write ("\n")

  for elem in types:
    class_name = elem.split ("=")[0].strip ()
    fields = elem.split("=")[1].strip ()
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
  output.write ("  std::string accept (%sVisitor &visitor)\n" % base_name)
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
  output.write ("struct %sVisitor\n" % base_name)
  output.write ("{\n")
  for elem in types:
    type_name = elem.split ("=")[0].strip ()
    output.write ("  virtual std::string visit" + type_name + base_name + " (" + \
                  type_name + " *" + base_name.lower () + ") = 0;\n")
  output.write ("};\n")


if __name__ == '__main__':
  main (len(sys.argv), sys.argv)
