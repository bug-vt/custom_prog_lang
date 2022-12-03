#ifndef STMT_HPP
#define STMT_HPP

#include "token.hpp"
#include "expr.hpp"
#include "func_table.hpp"
#include <string>
#include <vector>

struct Block;
struct Expression;
struct Function;
struct Return;
struct If;
struct While;
struct Goto;
struct Print;
struct Var;

struct StmtVisitor
{
  virtual std::string visitBlockStmt (Block *stmt) = 0;
  virtual std::string visitExpressionStmt (Expression *stmt) = 0;
  virtual std::string visitFunctionStmt (Function *stmt) = 0;
  virtual std::string visitReturnStmt (Return *stmt) = 0;
  virtual std::string visitIfStmt (If *stmt) = 0;
  virtual std::string visitWhileStmt (While *stmt) = 0;
  virtual std::string visitGotoStmt (Goto *stmt) = 0;
  virtual std::string visitPrintStmt (Print *stmt) = 0;
  virtual std::string visitVarStmt (Var *stmt) = 0;
};

struct Stmt
{
  virtual ~Stmt () { }
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

  virtual ~Block () { }

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

  virtual ~Expression () { }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitExpressionStmt (this);
  }

  Expr *expression;
};

struct Function : public Stmt
{
  Function (Token name, std::vector<Param> params, std::vector<Stmt*> body, int scope)
  {
    this->name = name;
    this->params = params;
    this->body = body;
    this->scope = scope;
  }

  virtual ~Function () { }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitFunctionStmt (this);
  }

  Token name;
  std::vector<Param> params;
  std::vector<Stmt*> body;
  int scope;
};

struct Return : public Stmt
{
  Return (Token keyword, Expr *value)
  {
    this->keyword = keyword;
    this->value = value;
  }

  virtual ~Return () { }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitReturnStmt (this);
  }

  Token keyword;
  Expr *value;
};

struct If : public Stmt
{
  If (Expr *condition, Stmt *thenBranch, Stmt *elseBranch)
  {
    this->condition = condition;
    this->thenBranch = thenBranch;
    this->elseBranch = elseBranch;
  }

  virtual ~If () { }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitIfStmt (this);
  }

  Expr *condition;
  Stmt *thenBranch;
  Stmt *elseBranch;
};

struct While : public Stmt
{
  While (Expr *condition, Stmt *body, Stmt *increment)
  {
    this->condition = condition;
    this->body = body;
    this->increment = increment;
  }

  virtual ~While () { }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitWhileStmt (this);
  }

  Expr *condition;
  Stmt *body;
  Stmt *increment;
};

struct Goto : public Stmt
{
  Goto (Token token)
  {
    this->token = token;
  }

  virtual ~Goto () { }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitGotoStmt (this);
  }

  Token token;
};

struct Print : public Stmt
{
  Print (Expr *expression)
  {
    this->expression = expression;
  }

  virtual ~Print () { }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitPrintStmt (this);
  }

  Expr *expression;
};

struct Var : public Stmt
{
  Var (Token name, Expr *initializer, int size, int scope)
  {
    this->name = name;
    this->initializer = initializer;
    this->size = size;
    this->scope = scope;
  }

  virtual ~Var () { }

  std::string accept (StmtVisitor &visitor)
  {
    return visitor.visitVarStmt (this);
  }

  Token name;
  Expr *initializer;
  int size;
  int scope;
};

#endif
