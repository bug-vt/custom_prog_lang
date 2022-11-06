#ifndef STMT_HPP
#define STMT_HPP

#include "token.hpp"
#include "expr.hpp"
#include <string>

struct Expression;
struct Var;

struct StmtVisitor
{
  virtual std::string visitExpressionStmt (Expression *stmt) = 0;
  virtual std::string visitVarStmt (Var *stmt) = 0;
};

struct Stmt
{
  virtual std::string accept (StmtVisitor &visitor)
  {
    throw std::runtime_error ("Visiting Expr base class");
  }
};

struct Expression : public Stmt
{
  Expression (Expr *expression)
  {
    this->expression = expression;
  }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitExpressionStmt (this);
  }

  Expr *expression;
};

struct Var : public Stmt
{
  Var (Token name, Expr *initializer)
  {
    this->name = name;
    this->initializer = initializer;
  }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitVarStmt (this);
  }

  Token name;
  Expr *initializer;
};

#endif
