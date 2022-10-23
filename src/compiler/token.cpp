#include "token.hpp"

using std::string;

string token2string (Token token)
{
  string out;
  switch (token)
  {
    case TOKEN_TYPE_EOF:
      out = "EOF";
      break;
    case TOKEN_TYPE_INT:
      out = "Int";
      break;
    case TOKEN_TYPE_FLOAT:
      out = "Float";
      break;
    case TOKEN_TYPE_STRING:
      out = "String";
      break;
    case TOKEN_TYPE_IDENT:
      out = "Identifier";
      break;
    case TOKEN_TYPE_OPEN_BRACKET:
      out = "Open bracket";
      break;
    case TOKEN_TYPE_CLOSE_BRACKET:
      out = "Close bracket";
      break;
    case TOKEN_TYPE_COMMA:
      out = "Comma";
      break;
    case TOKEN_TYPE_OPEN_BRACE:
      out = "Open brace";
      break;
    case TOKEN_TYPE_CLOSE_BRACE:
      out = "Close brace";
      break;
    case TOKEN_TYPE_OPEN_PAREN:
      out = "Open brace";
      break;
    case TOKEN_TYPE_CLOSE_PAREN:
      out = "Close brace";
      break;
    case TOKEN_TYPE_SEMICOLON:
      out = "Semi Colon";
      break;
    case TOKEN_TYPE_VAR:
      out = "var keyword";
      break;
    case TOKEN_TYPE_TRUE:
      out = "true keyword";
      break;
    case TOKEN_TYPE_FALSE:
      out = "false keyword";
      break;
    case TOKEN_TYPE_IF:
      out = "if keyword";
      break;
    case TOKEN_TYPE_ELSE:
      out = "else keyword";
      break;
    case TOKEN_TYPE_BREAK:
      out = "break keyword";
      break;
    case TOKEN_TYPE_CONTINUE:
      out = "continue keyword";
      break;
    case TOKEN_TYPE_FOR:
      out = "for keyword";
      break;
    case TOKEN_TYPE_WHILE:
      out = "while keyword";
      break;
    case TOKEN_TYPE_FUNC:
      out = "function keyword";
      break;
    case TOKEN_TYPE_RETURN:
      out = "return keyword";
      break;
    case TOKEN_TYPE_ASSIGN:
      out = "Assignment operator";
      break;
    case TOKEN_TYPE_ADD:
      out = "Addition operator";
      break;
    case TOKEN_TYPE_SUB:
      out = "Subtraction operator";
      break;
    case TOKEN_TYPE_MUL:
      out = "Multiplication operator";
      break;
    default:
      out = "Invalid";
  }
  return out;
}

