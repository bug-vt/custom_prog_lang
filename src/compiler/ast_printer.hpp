#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include <string>
#include <vector>
#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"


struct AstPrinter : public ExprVisitor, public StmtVisitor
{
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

  std::string visitLiteralExpr (Literal *expr)
  {
    if (expr->value.type != TOKEN_TYPE_INT)
      return "nil";

    return expr->value.lexeme;
  }

  std::string visitUnaryExpr (Unary *expr)
  {
    std::vector<Expr *> exprs = {expr->right};
    return parenthesize (expr->op.lexeme, exprs);
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
