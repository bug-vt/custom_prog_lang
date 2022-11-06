#ifndef EXPR_HPP
#define EXPR_HPP

#include "token.hpp"
#include "expr.hpp"
#include <string>

struct Assign;
struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct Variable;

struct ExprVisitor
{
  virtual std::string visitAssignExpr (Assign *expr) = 0;
  virtual std::string visitBinaryExpr (Binary *expr) = 0;
  virtual std::string visitGroupingExpr (Grouping *expr) = 0;
  virtual std::string visitLiteralExpr (Literal *expr) = 0;
  virtual std::string visitUnaryExpr (Unary *expr) = 0;
  virtual std::string visitVariableExpr (Variable *expr) = 0;
};

struct Expr
{
  virtual std::string accept (ExprVisitor &visitor)
  {
    throw std::runtime_error ("Visiting Expr base class");
  }
};

struct Assign : public Expr
{
  Assign (Token name, Expr *value)
  {
    this->name = name;
    this->value = value;
  }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitAssignExpr (this);
  }

  Token name;
  Expr *value;
};

struct Binary : public Expr
{
  Binary (Expr *left, Token op, Expr *right)
  {
    this->left = left;
    this->op = op;
    this->right = right;
  }

  std::string accept (ExprVisitor &visitor)
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

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitGroupingExpr (this);
  }

  Expr *expression;
};

struct Literal : public Expr
{
  Literal (Token value)
  {
    this->value = value;
  }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitLiteralExpr (this);
  }

  Token value;
};

struct Unary : public Expr
{
  Unary (Token op, Expr *right)
  {
    this->op = op;
    this->right = right;
  }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitUnaryExpr (this);
  }

  Token op;
  Expr *right;
};

struct Variable : public Expr
{
  Variable (Token name)
  {
    this->name = name;
  }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitVariableExpr (this);
  }

  Token name;
};

#endif
