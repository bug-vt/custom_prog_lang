#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include <string>
#include <vector>
#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "symbol_table.hpp"
#include "func_table.hpp"
#include <iostream>


struct AstPrinter : public ExprVisitor, public StmtVisitor
{
  // variable for assigning unique scope number 
  int scope;
  SymbolTable* global;
  SymbolTable* sym_table;  
  FuncTable func_table;

  AstPrinter ()
  {
    scope = 0;
    global = new SymbolTable ();
    sym_table = global;
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
      exit (EXIT_FAILURE);
    }

    return out;
  }

  std::string visitFunctionStmt (Function* stmt)
  {
    if (sym_table != global)
      throw std::runtime_error ("Function cannot be declared outside global scope");
    
    // function name
    func_table.addFunc (stmt->name.lexeme, stmt->params.size ());
    std::string out = "(Func " + stmt->name.lexeme;

    // parameters
    // Assigning new scope for function and adding parameters to symbol table
    SymbolTable* current = new SymbolTable (sym_table, ++scope);
    SymbolTable* previous = this->sym_table;
    this->sym_table = current;

    out += " (Params";
    for (Token param : stmt->params)
    {
      out += " " + param.lexeme;
      stmt->scope = sym_table->addSymbol (param.lexeme);
    }
    out += ")\n";
    
    // body
    for (Stmt* statement : stmt->body)
      out += statement->accept (*this) + "\n";
   
    // restore to global scope
    delete current;
    this->sym_table = previous;
  
    return out + ")";
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
    // check whether function was defined
    int arity = func_table.getFunc (((Variable*)expr->callee)->name.lexeme);

    out += parenthesize ("Args", expr->args);
    // check arity (number of expected arguments)
    if (expr->args.size () != arity)
      throw std::runtime_error ("Expected arity " + std::to_string (arity) + 
                                " but found " + std::to_string (expr->args.size ()));

    return out + ")";
  }

  std::string visitLiteralExpr (Literal* expr)
  {
    return expr->value.lexeme;
  }

  std::string visitVariableExpr (Variable* expr)
  {
    if (!func_table.isFunc (expr->name.lexeme))
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
