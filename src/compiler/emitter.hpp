#ifndef EMITTER_HPP
#define EMITTER_HPP

#include <string>
#include <vector>
#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "symbol_table.hpp"
#include <iostream>


struct Emitter : public ExprVisitor, public StmtVisitor
{
  struct LoopLabels
  {
    std::string start;
    std::string end;

    LoopLabels () { }
    LoopLabels (std::string start, std::string end)
    {
      this->start = start;
      this->end = end;
    }
  };
  // variable for assigning unique number to label
  int jump_target_id;  
  // store outer loop labels 
  std::vector<LoopLabels> loop_stack;

  Emitter () 
  { 
    jump_target_id = 0;
  }

  // watch out for undefined reference error when base class accept method is not defined.
  std::string walkAst (std::vector<Stmt*> statements)
  {
    std::string out = "";
    // two temporary variables to simulate general-purpose registers 
    out += "var _t0\n";
    out += "var _t1\n";
    out += "func main\n{\n";
    try
    {
      for (Stmt* statement : statements)
        out += statement->accept (*this) + "\n";
    }
    catch (std::runtime_error& err)
    {
      out = std::string (err.what ()) + "\n"; 
    }

    out += "}\n";
    return out;
  }

  std::string visitIfStmt (If* stmt)
  {
    std::string out = "";
    out += emit (stmt->condition);

    std::string false_block = getNextLabel ();
    out += "  pop _t0\n";
    out += "  je _t0, 0, " + false_block + "\n";

    out += stmt->thenBranch->accept (*this) + "\n";

    if (stmt->elseBranch != nullptr)
    {
      // at the end of the true block,
      // add unconditional jump to skip false block
      std::string skip_false_block = getNextLabel ();
      out += "  jmp " + skip_false_block + "\n";
     
      // place label for false block 
      out += false_block + ":\n";
      out += stmt->elseBranch->accept (*this) + "\n";
      // place label for skipping false block
      out += skip_false_block + ":\n";
    }
    else
      // place label for skipping true block
      out += false_block + ":\n";

    return out;
  }

  std::string visitWhileStmt (While* stmt)
  {
    std::string out = "";
    std::string start_label = getNextLabel ();
    std::string end_label = getNextLabel ();
    // place start label to loop back to start
    out += start_label + ":\n";

    out += emit (stmt->condition);
    // evaluate condition 
    out += "  pop _t0\n";
    out += "  je _t0, 0, " + end_label + "\n";

    // place innermost loop labels at the top of the stack
    LoopLabels labels (start_label, end_label);
    loop_stack.push_back (labels);

    out += stmt->body->accept (*this) + "\n";
    
    // restore stack back to what it was before 
    loop_stack.pop_back ();

    // loop back to start
    out += "  jmp " + start_label + "\n";
    
    // place end label to skip body in false condition
    out += end_label + ":\n";
    return out;
  }

  std::string visitGotoStmt (Goto* stmt)
  {
    std::string out = "";
    // get innermost loop labels from stack
    LoopLabels labels = loop_stack.back ();
    if (stmt->token.type == TOKEN_TYPE_BREAK)
      out = "  jmp " + labels.end + "\n";
    if (stmt->token.type == TOKEN_TYPE_CONTINUE)
      out = "  jmp " + labels.start + "\n";

    return out;
  }

  std::string visitExpressionStmt (Expression* stmt)
  {
    return emit (stmt->expression);
  }

  std::string visitPrintStmt (Print* stmt)
  {
    std::string out = "";
    out += emit (stmt->expression);
    out += "  pop _t0\n";
    out += "  print _t0\n";
    out += "  print \"\\n\"\n";
    return out;
  }

  std::string visitVarStmt (Var* stmt)
  {
    std::string name = stmt->name.lexeme + std::to_string (stmt->scope);
    std::string out = "  var " + name + "\n";
    if (stmt->initializer)
    {
      out += emit (stmt->initializer);
      out += "  pop _t0\n";
      out += "  mov " + name + ", _t0\n";
    }

    return out;
  }

  std::string visitBlockStmt (Block* stmt)
  {
    std::string out = "";
    
    for (Stmt* statement : stmt->statements)
      out += statement->accept (*this) + "\n";

    return out;
  }

  std::string visitAssignExpr (Assign* expr) 
  {
    std::string name = expr->name.lexeme + std::to_string (expr->scope);
    std::string out = "";
    out += emit (expr->value);
    out += "  pop _t0\n";
    out += "  mov " + name + ", _t0\n";
    out += "  push " + name + "\n";
    return out;
  }

  std::string visitBinaryExpr (Binary* expr) 
  {
    std::string out = "";
    out += emit (expr->left);
    out += emit (expr->right);

    out += "  pop _t1\n";
    out += "  pop _t0\n";

    switch (expr->op.type)
    {
      case TOKEN_TYPE_EQUAL:
        out += "  seq _t0, _t0, _t1\n";
        break;
      case TOKEN_TYPE_NOT_EQUAL:
        out += "  sne _t0, _t0, _t1\n";
        break;
      case TOKEN_TYPE_LESS:
        out += "  slt _t0, _t0, _t1\n";
        break;
      case TOKEN_TYPE_GREATER:
        out += "  sgt _t0, _t0, _t1\n";
        break;
      case TOKEN_TYPE_LESS_EQUAL:
        out += "  sle _t0, _t0, _t1\n";
        break;
      case TOKEN_TYPE_GREATER_EQUAL:
        out += "  sge _t0, _t0, _t1\n";
        break;
      case TOKEN_TYPE_ADD:
        out += "  add _t0, _t1\n";
        break;
      case TOKEN_TYPE_SUB:
        out += "  sub _t0, _t1\n";
        break;
      case TOKEN_TYPE_MUL:
        out += "  mul _t0, _t1\n";
        break;
      case TOKEN_TYPE_DIV:
        out += "  div _t0, _t1\n";
        break;
      default:
        throw std::runtime_error ("Invalid Binary operator");
    }

    out += "  push _t0\n";

    return out;
  }

  std::string visitGroupingExpr (Grouping* expr)
  {
    return emit (expr->expression);
  }

  std::string visitUnaryExpr (Unary* expr)
  {
    std::string out = "";
    out += emit (expr->right);

    out += "  pop _t0\n";

    switch (expr->op.type)
    {
      case TOKEN_TYPE_LOGICAL_NOT:
        out += "  not _t0\n";
        break;
      case TOKEN_TYPE_SUB:
        out += "  neg _t0\n";
        break;
      default:
        throw std::runtime_error ("Invalid Unary operator");
    }

    out += "  push _t0\n";

    return out;
  }

  std::string visitLiteralExpr (Literal* expr)
  {
    if (expr->value.type == TOKEN_TYPE_FALSE)
      return "  push 0\n";
    if (expr->value.type == TOKEN_TYPE_TRUE)
      return "  push 1\n";

    return "  push " + expr->value.lexeme + "\n";
  }

  std::string visitLogicalExpr (Logical* expr)
  {
    std::string out = "";
    // place left expression result into _t0
    out += emit (expr->left);
    out += "  pop _t0\n";

    std::string short_circuit = getNextLabel ();

    switch (expr->op.type)
    {
      case TOKEN_TYPE_LOGICAL_OR:
        // short circuit when left expression result is true
        out += "  sne _t1, _t0, 0\n";
        break;
      case TOKEN_TYPE_LOGICAL_AND:
        // short circuit when left expression result is false
        out += "  seq _t1, _t0, 0\n";
        break;
      default:
        throw std::runtime_error ("Invalid Binary operator");
    }
    // skip right expression if short circuit 
    out += "  je _t1, 1, " + short_circuit + "\n";
    // place right expression result to _t0 
    out += emit (expr->right);
    out += "  pop _t0\n";
   
    // place label for short circuiting
    out += short_circuit + ":\n";
    // pushing result from left expression if short_circuit,
    // otherwise pushing result from right expression
    out += "  push _t0\n";

    return out;
  }

  std::string visitVariableExpr (Variable* expr)
  {
    std::string name = expr->name.lexeme + std::to_string (expr->scope);
    return "  push " + name + "\n";
  }

  std::string emit (Expr* expr)
  {
    return expr->accept (*this);
  }

  std::string getNextLabel ()
  {
    return "_L" + std::to_string (jump_target_id++);
  }
};

#endif
