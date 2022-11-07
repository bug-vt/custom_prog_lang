#ifndef STMT_HPP
#define STMT_HPP

#include "token.hpp"
#include "expr.hpp"
#include <string>
#include <vector>

struct Block;
struct Expression;
struct Print;
struct Var;

struct StmtVisitor
{
  virtual std::string visitBlockStmt (Block *stmt) = 0;
  virtual std::string visitExpressionStmt (Expression *stmt) = 0;
  virtual std::string visitPrintStmt (Print *stmt) = 0;
  virtual std::string visitVarStmt (Var *stmt) = 0;
};

struct Stmt
{
  virtual std::string accept (StmtVisitor &visitor)
  {
    throw std::runtime_error ("Visiting Expr base class");
  }
};

struct Block : public Stmt
{
  Block (std::vector<Stmt*> statements)
  {
    this->statements = statements;
  }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitBlockStmt (this);
  }

  std::vector<Stmt*> statements;
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

struct Print : public Stmt
{
  Print (Expr *expression)
  {
    this->expression = expression;
  }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitPrintStmt (this);
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
