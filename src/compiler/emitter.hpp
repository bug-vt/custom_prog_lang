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
    std::string skip;
    std::string end;

    LoopLabels () { }
    LoopLabels (std::string skip, std::string end)
    {
      this->skip = skip;
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
    try
    {
      for (Stmt* statement : statements)
        out += statement->accept (*this) + "\n";
    }
    catch (std::runtime_error& err)
    {
      std::cout << err.what () << std::endl;
      exit (EXIT_FAILURE);
    }

    return out;
  }

  std::string visitFunctionStmt (Function* stmt)
  {
    // func directive and function name
    std::string out = "func " + stmt->name.lexeme + "\n";
    out += "{\n";
    
    // parameters
    for (Param param : stmt->params)
      out += "  param " + param.name.lexeme + std::to_string (stmt->scope) + "\n";
    
    // body
    for (Stmt* statement : stmt->body)
      out += statement->accept (*this) + "\n";

    out += "}\n";
    return out;
  }

  std::string visitReturnStmt (Return* stmt)
  {
    std::string out = "";
    // default return value is 0
    out += "  mov _retVal, 0\n";
    // otherwise, store return value to _retVal register
    if (stmt->value != nullptr)
    {
      out += emit (stmt->value);
      out += "  pop _retVal\n";
    }
    out += "  ret\n";
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
    std::string skip_label = getNextLabel ();
    std::string end_label = getNextLabel ();
    // place start label to loop back to start
    out += start_label + ":\n";

    out += emit (stmt->condition);
    // evaluate condition 
    out += "  pop _t0\n";
    out += "  je _t0, 0, " + end_label + "\n";

    // place innermost loop labels at the top of the stack
    LoopLabels labels (skip_label, end_label);
    loop_stack.push_back (labels);

    out += stmt->body->accept (*this) + "\n";
    
    // restore stack back to what it was before 
    loop_stack.pop_back ();
    
    // location where continue would jump to
    out += skip_label + ":\n";
    // only used when desugaring for loop
    if (stmt->increment != nullptr)
      out += stmt->increment->accept (*this) + "\n";

    // loop back to start
    out += "  jmp " + start_label + "\n";
    
    // place end label to skip body in false condition
    out += end_label + ":\n";
    return out;
  }

  std::string visitGotoStmt (Goto* stmt)
  {
    if (loop_stack.empty ())
      throw std::runtime_error ("Invalid use of " + stmt->token.lexeme + " outside loops");

    std::string out = "";
    // get innermost loop labels from stack
    LoopLabels labels = loop_stack.back ();
    if (stmt->token.type == TOKEN_TYPE_BREAK)
      out = "  jmp " + labels.end + "\n";
    if (stmt->token.type == TOKEN_TYPE_CONTINUE)
      out = "  jmp " + labels.skip + "\n";

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
    if (stmt->size > 1)
      name += "[" + std::to_string (stmt->size) + "]";
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

    // _t1 holds index, initialized to 0
    out += "  mov _t1, 0\n";

    // calcuate index if vairable is an array 
    if (expr->offset != nullptr)
    {
      out += emit (expr->offset);
      out += "  pop _t1\n";
      if (!expr->deref)
        name += "[_t1]";
    }
    // temporary store assigning value to _t0
    out += "  pop _t0\n";
    // dereference if vairable is a reference
    if (expr->deref)
      out += "  sw " + name + ", _t1, _t0\n";
    else
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

  std::string visitCallExpr (Call* expr)
  {
    std::string out = "";
    
    // push arguments to stack
    // should be push in right to left order
    for (int i = expr->args.size () - 1; i >= 0; i--)
      out += emit (expr->args[i]);
    
    if (expr->callee.lexeme == "time")
      out += "  time\n";
    else if (expr->callee.lexeme == "randint")
      out += "  randint\n";
    else if (expr->callee.lexeme == "exit")
      out += "  exit\n";
    else
      out += "  call " + expr->callee.lexeme + "\n";
    out += "  push _retVal\n";
    return out;
  }

  std::string visitRefExpr (Ref* expr)
  {
    std::string name = ((Variable*) expr->ref)->name.lexeme + 
                       std::to_string (((Variable*) expr->ref)->scope);
    std::string out = "";
    out += "  ref _t0, " + name + "\n";
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

  std::string visitVariableExpr (Variable* expr)
  {
    std::string out = "";
    std::string name = expr->name.lexeme + std::to_string (expr->scope);

    out += "  push " + name + "\n";
    return out;
  }

  std::string visitArrayElemExpr (ArrayElem* expr)
  {
    std::string out = "";
    std::string name = expr->name.lexeme + std::to_string (expr->scope);
    
    // calcuate array index
    out += emit (expr->offset);
    out += "  pop _t1\n";
    if (!expr->deref)
    {
      name += "[_t1]";
      out += "  push " + name + "\n";
    }
    // dereference if vairable is a reference
    else
    {
      out += "  lw _t0, " + name + ", _t1\n";
      out += "  push _t0\n";
    }

    return out;
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
