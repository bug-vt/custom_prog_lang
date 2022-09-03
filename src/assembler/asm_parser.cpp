#include "asm_parser.hpp"
#include "error.hpp"

using std::string;

AsmParser::AsmParser (string raw_source)
{
  lexer = AsmLexer (raw_source);
}

void AsmParser::readToken (Token req_token)
{
  if (lexer.getNextToken () != req_token)
  {
    string msg = token2string (req_token) + " expected";
    exitOnCodeError (msg); 
  }
}

