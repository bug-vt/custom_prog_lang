#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include <string>
#include <vector>
#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "symbol_table.hpp"


struct AstPrinter : public ExprVisitor, public StmtVisitor
{
  // general-purpose registers
  int tmp0;
  int tmp1;
  SymbolTable sym_table;  

  AstPrinter ()
  {
    // Add two temporary variables to simulate general-purpose registers 
    tmp0 = sym_table.addSymbol ("_T0");
    tmp1 = sym_table.addSymbol ("_T1");
  }

  std::string print (std::vector<Stmt*> statements)
  {
    // To do: undefined reference error when base class accept method is not defined.
    std::string out = "";
    for (int i = 0; i < (int) statements.size (); i++)
      out += statements[i]->accept (*this) + "\n";

    return out;
  }

  std::string visitExpressionStmt (Expression *stmt)
  {
    std::vector<Expr *> exprs = {stmt->expression};
    return parenthesize ("Stmt", exprs);
  }

  std::string visitVarStmt (Var *stmt)
  {
    std::vector<Expr *> exprs;
    if (stmt->initializer)
      exprs.push_back (stmt->initializer);

    sym_table.addSymbol (stmt->name.lexeme);
    return parenthesize ("Var " + stmt->name.lexeme, exprs);
  }

  std::string visitBinaryExpr (Binary *expr) 
  {
    std::vector<Expr *> exprs = {expr->left, expr->right};
    return parenthesize (expr->op.lexeme, exprs);
  }

  std::string visitGroupingExpr (Grouping *expr)
  {
    std::vector<Expr *> exprs = {expr->expression};
    return parenthesize ("group", exprs);
  }

  std::string visitUnaryExpr (Unary *expr)
  {
    std::vector<Expr *> exprs = {expr->right};
    return parenthesize (expr->op.lexeme, exprs);
  }

  std::string visitLiteralExpr (Literal *expr)
  {
    if (expr->value.type != TOKEN_TYPE_INT)
      return "nil";

    return expr->value.lexeme;
  }

  std::string visitVariableExpr (Variable *expr)
  {
    sym_table.getSymbol (expr->name.lexeme);
    return expr->name.lexeme;
  }

  std::string parenthesize (std::string name, std::vector<Expr *> exprs)
  {
    std::string out = "(" + name;
    for (int i = 0; i < (int) exprs.size (); i++)
    {
      out += " ";
      out += exprs[i]->accept (*this);
    }
    out += ")";

    return out;
  }
};

#endif
