#ifndef CLEANER_HPP
#define CLEANER_HPP

#include "token.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include <string>
#include <vector>

struct Cleaner : public ExprVisitor, public StmtVisitor
{
  Cleaner ()
  {
  }

  std::string cleanAst (std::vector<Stmt*> statements)  {
    for (Stmt* statement : statements)
      statement->accept (*this);

    return "";
  }

  std::string visitAssignExpr (Assign *expr)
  {
    traverse (expr->value);
    if (expr->offset)
      traverse (expr->offset);
    delete expr;
    return "";
  }

  std::string visitBinaryExpr (Binary *expr)
  {
    traverse (expr->left);
    traverse (expr->right);
    delete expr;
    return "";
  }

  std::string visitLogicalExpr (Logical *expr)
  {
    traverse (expr->left);
    traverse (expr->right);
    delete expr;
    return "";
  }

  std::string visitGroupingExpr (Grouping *expr)
  {
    traverse (expr->expression);
    delete expr;
    return "";
  }

  std::string visitUnaryExpr (Unary *expr)
  {
    traverse (expr->right);
    delete expr;
    return "";
  }

  std::string visitCallExpr (Call *expr)
  {
    for (Expr* arg : expr->args)
      traverse (arg);
    delete expr;
    return "";
  }

  std::string visitRefExpr (Ref *expr)
  {
    traverse (expr->ref);
    delete expr;
    return "";
  }

  std::string visitLiteralExpr (Literal *expr)
  {
    delete expr;
    return "";
  }

  std::string visitArrayElemExpr (ArrayElem *expr)
  {
    traverse (expr->offset);
    delete expr;
    return "";
  }

  std::string visitVariableExpr (Variable *expr)
  {
    delete expr;
    return "";
  }

  std::string visitBlockStmt (Block *stmt)
  {
    for (Stmt* statement : stmt->statements)
      statement->accept (*this);
    
    delete stmt;
    return "";
  }

  std::string visitExpressionStmt (Expression *stmt)
  {
    traverse (stmt->expression);
    delete stmt;
    return "";
  }

  std::string visitFunctionStmt (Function *stmt)
  {
    for (Stmt* statement : stmt->body)
      statement->accept (*this);

    delete stmt;
    return "";
  }

  std::string visitReturnStmt (Return *stmt)
  {
    if (stmt->value)
      traverse (stmt->value);

    delete stmt;
    return "";
  }

  std::string visitIfStmt (If *stmt)
  {
    traverse (stmt->condition);
    stmt->thenBranch->accept (*this);
    if (stmt->elseBranch)
      stmt->elseBranch->accept (*this);

    delete stmt;
    return "";
  }

  std::string visitWhileStmt (While *stmt)
  {
    traverse (stmt->condition);
    stmt->body->accept (*this);
    if (stmt->increment)
      stmt->increment->accept (*this);
    delete stmt;
    return "";
  }

  std::string visitGotoStmt (Goto *stmt)
  {
    delete stmt;
    return "";
  }

  std::string visitPrintStmt (Print *stmt)
  {
    traverse (stmt->expression);
    delete stmt;
    return "";
  }

  std::string visitVarStmt (Var *stmt)
  {
    if (stmt->initializer)
      traverse (stmt->initializer);
    delete stmt;
    return "";
  }

  std::string traverse (Expr* expr)
  {
    return expr->accept (*this);
  }
};
#endif
