#ifndef STMT_HPP
#define STMT_HPP

#include "icode.hpp"
#include "token.hpp"
#include "expr.hpp"
#include <string>

struct Expression;

struct StmtVisitor
{
  virtual std::string visitExpressionStmt (Expression *stmt) = 0;
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

#endif
