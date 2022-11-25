#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include <string>
#include <vector>
#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "symbol_table.hpp"
#include <iostream>


struct AstPrinter : public ExprVisitor, public StmtVisitor
{
  // variable for assigning unique scope number 
  int scope;
  SymbolTable* sym_table;  
  std::unordered_map<std::string, int> func_table;

  AstPrinter ()
  {
    scope = 0;
    sym_table = new SymbolTable ();
    // reserve two temporary variables to simulate general-purpose registers 
    sym_table->addSymbol ("_t0");
    sym_table->addSymbol ("_t1");
  }

  // watch out for undefined reference error when base class accept method is not defined.
  std::string print (std::vector<Stmt*> statements)
  {
    std::string out = "";
    try
    {
      for (Stmt* statement : statements)
        out += statement->accept (*this) + "\n";
    }
    catch (std::runtime_error& err)
    {
      std::cout << err.what () << std::endl; 
      exit (-1);
    }

    return out;
  }

  std::string visitIfStmt (If* stmt)
  {
    std::vector<Expr *> exprs = {stmt->condition};
    std::string out = "(If " + parenthesize ("Condition", exprs);
    out += stmt->thenBranch->accept (*this) + "\n";
    out += ")\n";
    if (stmt->elseBranch != nullptr)
    {
      out += "(Else\n";
      out += stmt->elseBranch->accept (*this) + "\n";
      out += ")\n";
    }
    return out;
  }

  std::string visitWhileStmt (While* stmt)
  {
    std::vector<Expr *> exprs = {stmt->condition};
    std::string out = "(While " + parenthesize ("Condition", exprs);
    out += stmt->body->accept (*this) + "\n";
    // only used when desugaring for loop
    if (stmt->increment != nullptr)
      out += stmt->increment->accept (*this) + "\n";
    out += ")\n";
    return out;
  }

  std::string visitGotoStmt (Goto* stmt)
  {
    return "(" + stmt->token.lexeme + ")\n";
  }

  std::string visitExpressionStmt (Expression* stmt)
  {
    std::vector<Expr *> exprs = {stmt->expression};
    return parenthesize ("Expr", exprs);
  }

  std::string visitPrintStmt (Print* stmt)
  {
    std::vector<Expr *> exprs = {stmt->expression};
    return parenthesize ("Print", exprs);
  }

  std::string visitVarStmt (Var* stmt)
  {
    std::vector<Expr *> exprs;
    if (stmt->initializer)
      exprs.push_back (stmt->initializer);

    stmt->scope = sym_table->addSymbol (stmt->name.lexeme);
    return parenthesize ("Var " + stmt->name.lexeme, exprs);
  }

  std::string visitBlockStmt (Block* stmt)
  {
    std::string out = "";
    SymbolTable* current = new SymbolTable (sym_table, ++scope);
    SymbolTable* previous = this->sym_table;
    this->sym_table = current;
    
    for (Stmt* statement : stmt->statements)
      out += statement->accept (*this) + "\n";
    
    delete current;
    this->sym_table = previous;
  
    // remove extra newline at the end of the string
    return out = out.substr (0, out.length () - 1);
  }

  std::string visitAssignExpr (Assign* expr) 
  {
    std::vector<Expr *> exprs = {expr->value};
    expr->scope = sym_table->getScope (expr->name.lexeme);
    return parenthesize (expr->name.lexeme + "=", exprs);
  }

  std::string visitBinaryExpr (Binary* expr) 
  {
    std::vector<Expr *> exprs = {expr->left, expr->right};
    return parenthesize (expr->op.lexeme, exprs);
  }

  std::string visitLogicalExpr (Logical* expr)
  {
    std::vector<Expr *> exprs = {expr->left, expr->right};
    return parenthesize (expr->op.lexeme, exprs);
  }

  std::string visitGroupingExpr (Grouping* expr)
  {
    std::vector<Expr *> exprs = {expr->expression};
    return parenthesize ("group", exprs);
  }

  std::string visitUnaryExpr (Unary* expr)
  {
    std::vector<Expr *> exprs = {expr->right};
    return parenthesize (expr->op.lexeme, exprs);
  }

  std::string visitCallExpr (Call* expr)
  {
    std::string out = "(";
    std::vector<Expr *> exprs = {expr->callee};
    out += parenthesize ("Callee", exprs);
    // To do: check whether function was defined

    out += parenthesize ("Args", expr->args);
    // To do: check arity (number of expected arguments)

    return out + ")";
  }

  std::string visitLiteralExpr (Literal* expr)
  {
    return expr->value.lexeme;
  }

  std::string visitVariableExpr (Variable* expr)
  {
    expr->scope = sym_table->getScope (expr->name.lexeme);
    return expr->name.lexeme;
  }

  std::string parenthesize (std::string name, std::vector<Expr*> exprs)
  {
    std::string out = "(" + name;
    for (Expr* expr : exprs)
    {
      out += " ";
      out += expr->accept (*this);
    }
    out += ")";

    return out;
  }
};

#endif
