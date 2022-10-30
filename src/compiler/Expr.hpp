#include "icode.hpp"
#include "token.hpp"

template <typename T>
class Visitor;

class Expr
{
};

class Binary : public Expr
{
  Binary (Expr left, Token op, Expr right)
  {
    this->left = left;
    this->op = op;
    this->right = right;
  }

  template <typename T>
  T accept (Visitor<T> visitor)
  {
    return visitor.visitBinaryExpr (*this);
  }

  Expr left;
  Token op;
  Expr right;
};

class Grouping : public Expr
{
  Grouping (Expr expression)
  {
    this->expression = expression;
  }

  template <typename T>
  T accept (Visitor<T> visitor)
  {
    return visitor.visitGroupingExpr (*this);
  }

  Expr expression;
};

class Literal : public Expr
{
  Literal (Op value)
  {
    this->value = value;
  }

  template <typename T>
  T accept (Visitor<T> visitor)
  {
    return visitor.visitLiteralExpr (*this);
  }

  Op value;
};

class Unary : public Expr
{
  Unary (Token op, Expr right)
  {
    this->op = op;
    this->right = right;
  }

  template <typename T>
  T accept (Visitor<T> visitor)
  {
    return visitor.visitUnaryExpr (*this);
  }

  Token op;
  Expr right;
};

template <typename T>
class Visitor
{
  virtual T visitBinaryExpr (Binary expr) = 0;
  virtual T visitGroupingExpr (Grouping expr) = 0;
  virtual T visitLiteralExpr (Literal expr) = 0;
  virtual T visitUnaryExpr (Unary expr) = 0;
};
