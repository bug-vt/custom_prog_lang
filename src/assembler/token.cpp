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
    case TOKEN_TYPE_NEWLINE:
      out = "New line";
      break;
    case TOKEN_TYPE_INSTR:
      out = "Instruction";
      break;
    case TOKEN_TYPE_SETSTACKSIZE:
      out = "setStackSize";
      break;
    case TOKEN_TYPE_VAR:
      out = "var";
      break;
    case TOKEN_TYPE_FUNC:
      out = "func";
      break;
    case TOKEN_TYPE_PARAM:
      out = "param";
      break;
    case TOKEN_TYPE_REG_RETVAL:
      out = "_retVal";
      break;
    default:
      out = "Invalid";
  }
  return out;
}
