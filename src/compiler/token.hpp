#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

typedef int Token;
enum TokenType {TOKEN_TYPE_EOF,
                TOKEN_TYPE_INT,
                TOKEN_TYPE_FLOAT,
                TOKEN_TYPE_STRING,
                TOKEN_TYPE_IDENT,
                TOKEN_TYPE_OPEN_BRACKET,
                TOKEN_TYPE_CLOSE_BRACKET,
                TOKEN_TYPE_COMMA,
                TOKEN_TYPE_OPEN_BRACE,
                TOKEN_TYPE_CLOSE_BRACE,
                TOKEN_TYPE_OPEN_PAREN,
                TOKEN_TYPE_CLOSE_PAREN,
                TOKEN_TYPE_SEMICOLON,
                TOKEN_TYPE_VAR,
                TOKEN_TYPE_TRUE,
                TOKEN_TYPE_FALSE,
                TOKEN_TYPE_IF,
                TOKEN_TYPE_ELSE,
                TOKEN_TYPE_BREAK,
                TOKEN_TYPE_CONTINUE,
                TOKEN_TYPE_FOR,
                TOKEN_TYPE_WHILE,
                TOKEN_TYPE_FUNC,
                TOKEN_TYPE_RETURN,
                TOKEN_TYPE_INVALID};

std::string token2string (Token token);

#endif
