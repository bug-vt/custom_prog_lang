#ifndef _TOKEN
#define _TOKEN

#include <string>
#include "instr_lookup.hpp"
#include "instruction.hpp"

typedef int Token;
enum TokenType {TOKEN_TYPE_EOF,
                TOKEN_TYPE_INT,
                TOKEN_TYPE_FLOAT,
                TOKEN_TYPE_STRING,
                TOKEN_TYPE_IDENT,
                TOKEN_TYPE_COLON,
                TOKEN_TYPE_OPEN_BRACKET,
                TOKEN_TYPE_CLOSE_BRACKET,
                TOKEN_TYPE_COMMA,
                TOKEN_TYPE_OPEN_BRACE,
                TOKEN_TYPE_CLOSE_BRACE,
                TOKEN_TYPE_NEWLINE,
                TOKEN_TYPE_INSTR,
                TOKEN_TYPE_SETSTACKSIZE,
                TOKEN_TYPE_VAR,
                TOKEN_TYPE_FUNC,
                TOKEN_TYPE_PARAM,
                TOKEN_TYPE_REG_RETVAL,
                TOKEN_TYPE_INVALID};

std::string token2string (Token token);
OpBitFlags token2bitflag (Token token);
OpType token2op (Token token);

#endif
