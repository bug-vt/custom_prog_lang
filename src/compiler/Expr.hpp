#ifndef EXPR_HPP
#define EXPR_HPP

#include "icode.hpp"
#include "token.hpp"
#include <string>

class Expr;
class Binary;
class Grouping;
class Literal;
class Unary;

struct Visitor
{
  virtual std::string visitBinaryExpr (Binary *expr) = 0;
  virtual std::string visitGroupingExpr (Grouping *expr) = 0;
  virtual std::string visitLiteralExpr (Literal *expr) = 0;
  virtual std::string visitUnaryExpr (Unary *expr) = 0;
};

struct Expr
{
  virtual std::string accept (Visitor &visitor) { return ""; }
};

struct Binary : public Expr
{
  Binary (Expr *left, Token op, Expr *right)
  {
    this->left = left;
    this->op = op;
    this->right = right;
  }

  virtual std::string accept (Visitor &visitor)
  {
    return visitor.visitBinaryExpr (this);
  }

  Expr *left;
  Token op;
  Expr *right;
};

struct Grouping : public Expr
{
  Grouping (Expr *expression)
  {
    this->expression = expression;
  }

  virtual std::string accept (Visitor &visitor)
  {
    return visitor.visitGroupingExpr (this);
  }

  Expr *expression;
};

struct Literal : public Expr
{
  Literal (Op value)
  {
    this->value = value;
  }

  virtual std::string accept (Visitor &visitor)
  {
    return visitor.visitLiteralExpr (this);
  }

  Op value;
};

struct Unary : public Expr
{
  Unary (Token op, Expr *right)
  {
    this->op = op;
    this->right = right;
  }

  virtual std::string accept (Visitor &visitor)
  {
    return visitor.visitUnaryExpr (this);
  }

  Token op;
  Expr *right;
};

#endif
