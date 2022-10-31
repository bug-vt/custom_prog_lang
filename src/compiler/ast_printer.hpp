#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include <string>
#include "icode.hpp"
#include <vector>
#include "expr.hpp"
#include "token.hpp"


struct AstPrinter : public Visitor
{
  std::string print (Expr *expr)
  {
    // To do: undefined reference error when base class accept method is not defined.
    return expr->accept (*this);
  }

  std::string visitBinaryExpr (Binary *expr) 
  {
    std::vector<Expr *> exprs = {expr->left, expr->right};
    return parenthesize (token2string (expr->op), exprs);
  }

  std::string visitGroupingExpr (Grouping *expr)
  {
    std::vector<Expr *> exprs = {expr->expression};
    return parenthesize ("group", exprs);
  }

  std::string visitLiteralExpr (Literal *expr)
  {
    if (expr->value.type != OP_TYPE_INT)
      return "nil";

    return std::to_string (expr->value.int_literal);
  }

  std::string visitUnaryExpr (Unary *expr)
  {
    std::vector<Expr *> exprs = {expr->right};
    return parenthesize (token2string (expr->op), exprs);
  }

  std::string parenthesize (std::string name, std::vector<Expr *> exprs)
  {
    std::string out = "(" + name;
    for (int i = 0; i < exprs.size (); i++)
    {
      out += " ";
      out += exprs[i]->accept (*this);
    }
    out += ")";

    return out;
  }
};

#endif
