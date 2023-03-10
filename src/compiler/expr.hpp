#ifndef EXPR_HPP
#define EXPR_HPP

#include "token.hpp"
#include "expr.hpp"
#include "func_table.hpp"
#include <string>
#include <vector>

struct Assign;
struct Binary;
struct Logical;
struct Grouping;
struct Unary;
struct Call;
struct Ref;
struct Literal;
struct ArrayElem;
struct Variable;

struct ExprVisitor
{
  virtual std::string visitAssignExpr (Assign *expr) = 0;
  virtual std::string visitBinaryExpr (Binary *expr) = 0;
  virtual std::string visitLogicalExpr (Logical *expr) = 0;
  virtual std::string visitGroupingExpr (Grouping *expr) = 0;
  virtual std::string visitUnaryExpr (Unary *expr) = 0;
  virtual std::string visitCallExpr (Call *expr) = 0;
  virtual std::string visitRefExpr (Ref *expr) = 0;
  virtual std::string visitLiteralExpr (Literal *expr) = 0;
  virtual std::string visitArrayElemExpr (ArrayElem *expr) = 0;
  virtual std::string visitVariableExpr (Variable *expr) = 0;
};

struct Expr
{
  virtual ~Expr () { }
  virtual std::string accept (ExprVisitor &visitor)
  {
    throw std::runtime_error ("Visiting Expr base class");
  }
};

struct Assign : public Expr
{
  Assign (Token name, Expr *value, Expr *offset, int scope, bool deref)
  {
    this->name = name;
    this->value = value;
    this->offset = offset;
    this->scope = scope;
    this->deref = deref;
  }

  virtual ~Assign () { }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitAssignExpr (this);
  }

  Token name;
  Expr *value;
  Expr *offset;
  int scope;
  bool deref;
};

struct Binary : public Expr
{
  Binary (Expr *left, Token op, Expr *right)
  {
    this->left = left;
    this->op = op;
    this->right = right;
  }

  virtual ~Binary () { }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitBinaryExpr (this);
  }

  Expr *left;
  Token op;
  Expr *right;
};

struct Logical : public Expr
{
  Logical (Expr *left, Token op, Expr *right)
  {
    this->left = left;
    this->op = op;
    this->right = right;
  }

  virtual ~Logical () { }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitLogicalExpr (this);
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

  virtual ~Grouping () { }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitGroupingExpr (this);
  }

  Expr *expression;
};

struct Unary : public Expr
{
  Unary (Token op, Expr *right)
  {
    this->op = op;
    this->right = right;
  }

  virtual ~Unary () { }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitUnaryExpr (this);
  }

  Token op;
  Expr *right;
};

struct Call : public Expr
{
  Call (Token callee, Token paren, std::vector<Expr*> args)
  {
    this->callee = callee;
    this->paren = paren;
    this->args = args;
  }

  virtual ~Call () { }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitCallExpr (this);
  }

  Token callee;
  Token paren;
  std::vector<Expr*> args;
};

struct Ref : public Expr
{
  Ref (Expr *ref)
  {
    this->ref = ref;
  }

  virtual ~Ref () { }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitRefExpr (this);
  }

  Expr *ref;
};

struct Literal : public Expr
{
  Literal (Token value)
  {
    this->value = value;
  }

  virtual ~Literal () { }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitLiteralExpr (this);
  }

  Token value;
};

struct ArrayElem : public Expr
{
  ArrayElem (Token name, Expr *offset, int scope, bool deref)
  {
    this->name = name;
    this->offset = offset;
    this->scope = scope;
    this->deref = deref;
  }

  virtual ~ArrayElem () { }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitArrayElemExpr (this);
  }

  Token name;
  Expr *offset;
  int scope;
  bool deref;
};

struct Variable : public Expr
{
  Variable (Token name, int scope, bool deref)
  {
    this->name = name;
    this->scope = scope;
    this->deref = deref;
  }

  virtual ~Variable () { }

  std::string accept (ExprVisitor &visitor)
  {
    return visitor.visitVariableExpr (this);
  }

  Token name;
  int scope;
  bool deref;
};

#endif
