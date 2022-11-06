#include "token.hpp"

using std::string;

string token2string (TokenType token)
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
      out = "=";
      break;
    case TOKEN_TYPE_ADD:
      out = "+";
      break;
    case TOKEN_TYPE_SUB:
      out = "-";
      break;
    case TOKEN_TYPE_MUL:
      out = "*";
      break;
    case TOKEN_TYPE_DIV:
      out = "/";
      break;
    case TOKEN_TYPE_MOD:
      out = "%";
      break;
    case TOKEN_TYPE_EXP:
      out = "^";
      break;
    case TOKEN_TYPE_INC:
      out = "++";
      break;
    case TOKEN_TYPE_DEC:
      out = "--";
      break;
    case TOKEN_TYPE_ASSIGN_ADD:
      out = "+=";
      break;
    case TOKEN_TYPE_ASSIGN_SUB:
      out = "-=";
      break;
    case TOKEN_TYPE_ASSIGN_MUL:
      out = "*=";
      break;
    case TOKEN_TYPE_ASSIGN_DIV:
      out = "/=";
      break;
    case TOKEN_TYPE_ASSIGN_MOD:
      out = "%=";
      break;
    case TOKEN_TYPE_ASSIGN_EXP:
      out = "^=";
      break;
    case TOKEN_TYPE_BITWISE_AND:
      out = "&";
      break;
    case TOKEN_TYPE_BITWISE_OR:
      out = "|";
      break;
    case TOKEN_TYPE_BITWISE_XOR:
      out = "#";
      break;
    case TOKEN_TYPE_BITWISE_NOT:
      out = "~";
      break;
    case TOKEN_TYPE_BITWISE_SHIFT_LEFT:
      out = "<<";
      break;
    case TOKEN_TYPE_BITWISE_SHIFT_RIGHT:
      out = ">>";
      break;
    case TOKEN_TYPE_ASSIGN_AND:
      out = "&=";
      break;
    case TOKEN_TYPE_ASSIGN_OR:
      out = "|=";
      break;
    case TOKEN_TYPE_ASSIGN_XOR:
      out = "#=";
      break;
    case TOKEN_TYPE_ASSIGN_SHIFT_LEFT:
      out = "<<=";
      break;
    case TOKEN_TYPE_ASSIGN_SHIFT_RIGHT:
      out = ">>=";
      break;
    case TOKEN_TYPE_LOGICAL_AND:
      out = "&&";
      break;
    case TOKEN_TYPE_LOGICAL_OR:
      out = "||";
      break;
    case TOKEN_TYPE_LOGICAL_NOT:
      out = "!";
      break;
    case TOKEN_TYPE_EQUAL:
      out = "==";
      break;
    case TOKEN_TYPE_NOT_EQUAL:
      out = "!=";
      break;
    case TOKEN_TYPE_LESS:
      out = "<";
      break;
    case TOKEN_TYPE_GREATER:
      out = ">";
      break;
    case TOKEN_TYPE_LESS_EQUAL:
      out = "<=";
      break;
    case TOKEN_TYPE_GREATER_EQUAL:
      out = ">=";
      break;
    default:
      out = "Invalid";
  }
  return out;
}

