#ifndef EMITTER_HPP
#define EMITTER_HPP

#include <string>
#include <vector>
#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "symbol_table.hpp"
#include <iostream>


struct Emitter : public ExprVisitor, public StmtVisitor
{
  SymbolTable* sym_table;  

  Emitter ()
  {
    sym_table = new SymbolTable ();
  }

  std::string walkAst (std::vector<Stmt*> statements)
  {
    // To do: undefined reference error when base class accept method is not defined.
    std::string out = "";
    out += "var _t0\n";
    out += "var _t1\n";
    out += "func main\n{\n";
    try
    {
      for (Stmt* statement : statements)
        out += statement->accept (*this) + "\n";
    }
    catch (std::runtime_error& err)
    {
      out = std::string (err.what ()) + "\n"; 
    }

    out += "}\n";
    return out;
  }

  std::string visitExpressionStmt (Expression* stmt)
  {
    return emit (stmt->expression);
  }

  std::string visitPrintStmt (Print* stmt)
  {
    std::string out = "";
    out += emit (stmt->expression);
    out += "  pop _t0\n";
    out += "  print _t0\n";
    out += "  print \"\\n\"\n";
    return out;
  }

  std::string visitVarStmt (Var* stmt)
  {
    char scope [100];
    snprintf (scope, 100, "%p", sym_table);
    std::string out = "  var " + stmt->name.lexeme + std::string (scope) + "\n";
    if (stmt->initializer)
    {
      out += emit (stmt->initializer);
      out += "  pop _t0\n";
      out += "  mov " + stmt->name.lexeme + std::string (scope) + ", _t0\n";
    }

    return out;
  }

  std::string visitBlockStmt (Block* stmt)
  {
    std::string out = "";
    SymbolTable* current = new SymbolTable (sym_table);
    SymbolTable* previous = this->sym_table;
    this->sym_table = current;
    
    for (Stmt* statement : stmt->statements)
      out += statement->accept (*this) + "\n";
   
    delete current;
    this->sym_table = previous;

    return out;
  }

  std::string visitAssignExpr (Assign* expr) 
  {
    char scope [100];
    snprintf (scope, 100, "%p", sym_table);

    std::string out = "";
    out += emit (expr->value);
    out += "  pop _t0\n";
    out += "  mov " + expr->name.lexeme + std::string (scope) + ", _t0\n";
    return out;
  }

  std::string visitBinaryExpr (Binary* expr) 
  {
    std::string out = "";
    out += emit (expr->left);
    out += emit (expr->right);

    out += "  pop _t1\n";
    out += "  pop _t0\n";

    switch (expr->op.type)
    {
      case TOKEN_TYPE_EQUAL:
        out += "  seq _t0, _t1\n";
        break;
      case TOKEN_TYPE_NOT_EQUAL:
        out += "  sne _t0, _t1\n";
        break;
      case TOKEN_TYPE_LESS:
        out += "  slt _t0, _t1\n";
        break;
      case TOKEN_TYPE_GREATER:
        out += "  sgt _t0, _t1\n";
        break;
      case TOKEN_TYPE_LESS_EQUAL:
        out += "  sle _t0, _t1\n";
        break;
      case TOKEN_TYPE_GREATER_EQUAL:
        out += "  sge _t0, _t1\n";
        break;
      case TOKEN_TYPE_ADD:
        out += "  add _t0, _t1\n";
        break;
      case TOKEN_TYPE_SUB:
        out += "  sub _t0, _t1\n";
        break;
      case TOKEN_TYPE_MUL:
        out += "  mul _t0, _t1\n";
        break;
      case TOKEN_TYPE_DIV:
        out += "  div _t0, _t1\n";
        break;
      default:
        throw std::runtime_error ("Invalid Binary operator");
    }

    out += "  push _t0\n";

    return out;
  }

  std::string visitGroupingExpr (Grouping* expr)
  {
    return emit (expr->expression);
  }

  std::string visitUnaryExpr (Unary* expr)
  {
    std::string out = "";
    out += emit (expr->right);

    out += "  pop _t0\n";

    switch (expr->op.type)
    {
      case TOKEN_TYPE_LOGICAL_NOT:
        out += "  not _t0\n";
        break;
      case TOKEN_TYPE_SUB:
        out += "  neg _t0\n";
        break;
      default:
        throw std::runtime_error ("Invalid Unary operator");
    }

    out += "  push _t0\n";

    return out;
  }

  std::string visitLiteralExpr (Literal* expr)
  {
    return "  push " + expr->value.lexeme + "\n";
  }

  std::string visitVariableExpr (Variable* expr)
  {
    char scope [100];
    snprintf (scope, 100, "%p", sym_table);
    return "  push " + expr->name.lexeme + std::string (scope) + "\n";
  }

  std::string emit (Expr* expr)
  {
    return expr->accept (*this);
  }
};

#endif
