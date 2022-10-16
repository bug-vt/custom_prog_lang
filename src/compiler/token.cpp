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
    case TOKEN_TYPE_COLON:
      out = "Colon";
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
    case TOKEN_TYPE_VAR:
      out = "var";
      break;
    case TOKEN_TYPE_TRUE:
      out = "true";
      break;
    case TOKEN_TYPE_FALSE:
      out = "false";
      break;
    case TOKEN_TYPE_IF:
      out = "if";
      break;
    case TOKEN_TYPE_ELSE:
      out = "else";
      break;
    case TOKEN_TYPE_BREAK:
      out = "break";
      break;
    case TOKEN_TYPE_CONTINUE:
      out = "continue";
      break;
    case TOKEN_TYPE_FOR:
      out = "for";
      break;
    case TOKEN_TYPE_WHILE:
      out = "while";
      break;
    case TOKEN_TYPE_FUNC:
      out = "func";
      break;
    case TOKEN_TYPE_RETURN:
      out = "return";
      break;
    default:
      out = "Invalid";
  }
  return out;
}

