#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include "../stack.hpp"


TEST_CASE ("Test stack initialization", "[stack]")
{
  Stack stack (1024);
  stack.reset ();
  REQUIRE_THROWS (stack.getValue (0));  
  REQUIRE_THROWS (stack.getValue (-1));  
}

TEST_CASE ("Test stack getValue", "[stack]")
{
  Stack stack (1024);
  stack.reset ();
  stack.pushFrame (1);
  REQUIRE (stack.getValue (0).type == OP_TYPE_INVALID);  
  REQUIRE (stack.getValue (-1).type == OP_TYPE_INVALID);  

  Value val;
  val.type = OP_TYPE_INT;
  stack.setValue (0, val);
  REQUIRE (stack.getValue (0).type == OP_TYPE_INT);  
  REQUIRE (stack.getValue (-1).type == OP_TYPE_INT);  
}

TEST_CASE ("Test stack pushFrame", "[stack]")
{
  Stack stack (1024);
  stack.reset ();
  stack.pushFrame (0);
  stack.pushFrame (0, -1, 0);
  REQUIRE (stack.getValue (-1).func_index == 0);  
  REQUIRE (stack.getValue (-2).instr_index == -1);  
  REQUIRE_THROWS (stack.getValue (-3));  
}

TEST_CASE ("Test stack popFrame", "[stack]")
{
  Stack stack (1024);
  stack.reset ();
  stack.pushFrame (0);
  stack.pushFrame (4, -1, 0);
  stack.popFrame (6);
  REQUIRE_THROWS (stack.getValue (-1));  
}

TEST_CASE ("Simulate nested function calls", "[stack]")
{
  Stack stack (1024);
  stack.reset ();

  // pushing main stack frame
  stack.pushFrame (0);
  stack.pushFrame (0, -1, 0);
  REQUIRE (stack.getValue (-1).func_index == 0);  
  CHECK (stack.getValue (-1).offset_index == 0);  

  // pushing function 1 stack frame
  stack.pushFrame (0, 5, 1);
  REQUIRE (stack.getValue (-1).func_index == 1);  
  CHECK (stack.getValue (-1).offset_index == 2);  

  // pushing function 2 stack frame
  stack.pushFrame (0, 7, 2);
  REQUIRE (stack.getValue (-1).func_index == 2); 
  CHECK (stack.getValue (-1).offset_index == 4);  
 
  // return address from function 2
  REQUIRE_NOTHROW (stack.getValue (-2));  
  REQUIRE (stack.getValue (-2).instr_index == 7);  
  stack.popFrame (2);

  // return address from function 1
  REQUIRE_NOTHROW (stack.getValue (-2));  
  REQUIRE (stack.getValue (-2).instr_index == 5);  
  stack.popFrame (2);

  // return address from main
  REQUIRE_NOTHROW (stack.getValue (-2));  
  REQUIRE (stack.getValue (-2).instr_index == -1);  
  stack.popFrame (2);
}

TEST_CASE ("Test frame pointer correctness", "[stack]")
{
  Stack stack (1024);
  stack.reset ();

  // pushing main stack frame
  stack.pushFrame (3);
  stack.pushFrame (0, -1, 0);
  REQUIRE (stack.getValue (-1).func_index == 0);  
  CHECK (stack.getValue (-1).offset_index == 3);  

  // push dummy value
  Value dummy1;
  dummy1.func_index = 77;
  stack.push (dummy1);
  // stack frame should stay the same 
  REQUIRE (stack.getValue (-1).func_index == 0);  

  // pushing function 1 stack frame
  stack.pushFrame (2, 2, 1);
  Value dummy2;
  dummy2.func_index = 66;
  stack.push (dummy2);
  REQUIRE (stack.getValue (-1).func_index == 1);  
  CHECK (stack.getValue (-1).offset_index == 5);  

  // pop stack frame for function 1. should silently pop the dummy value
  // in the current stack frame.
  stack.popFrame (4);
  REQUIRE (stack.getValue (-1).func_index == 0);  
  CHECK (stack.getValue (-1).offset_index == 3);  

  stack.popFrame (2);
}
