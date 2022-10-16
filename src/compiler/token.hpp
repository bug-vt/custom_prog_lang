#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

typedef int Token;
enum TokenType {TOKEN_TYPE_EOF,
                // types
                TOKEN_TYPE_INT,
                TOKEN_TYPE_FLOAT,
                TOKEN_TYPE_STRING,
                TOKEN_TYPE_IDENT,
                // delimiters
                TOKEN_TYPE_OPEN_BRACKET,
                TOKEN_TYPE_CLOSE_BRACKET,
                TOKEN_TYPE_COMMA,
                TOKEN_TYPE_OPEN_BRACE,
                TOKEN_TYPE_CLOSE_BRACE,
                TOKEN_TYPE_OPEN_PAREN,
                TOKEN_TYPE_CLOSE_PAREN,
                TOKEN_TYPE_SEMICOLON,
                // reserved words
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
                // operators
                // arithmetic
                TOKEN_TYPE_ASSIGN,
                TOKEN_TYPE_ADD,
                TOKEN_TYPE_SUB,
                TOKEN_TYPE_MUL,
                TOKEN_TYPE_DIV,
                TOKEN_TYPE_MOD,
                TOKEN_TYPE_EXP,
                TOKEN_TYPE_INC,
                TOKEN_TYPE_DEC,
                TOKEN_TYPE_ASSIGN_ADD,
                TOKEN_TYPE_ASSIGN_SUB,
                TOKEN_TYPE_ASSIGN_MUL,
                TOKEN_TYPE_ASSIGN_DIV,
                TOKEN_TYPE_ASSIGN_MOD,
                TOKEN_TYPE_ASSIGN_EXP,
                // bitwise
                TOKEN_TYPE_BITWISE_AND,
                TOKEN_TYPE_BITWISE_OR,
                TOKEN_TYPE_BITWISE_XOR,
                TOKEN_TYPE_BITWISE_NOT,
                TOKEN_TYPE_BITWISE_SHIFT_LEFT,
                TOKEN_TYPE_BITWISE_SHIFT_RIGHT,
                TOKEN_TYPE_ASSIGN_AND,
                TOKEN_TYPE_ASSIGN_OR,
                TOKEN_TYPE_ASSIGN_XOR,
                TOKEN_TYPE_ASSIGN_SHIFT_LEFT,
                TOKEN_TYPE_ASSIGN_SHIFT_RIGHT,
                // logical
                TOKEN_TYPE_LOGICAL_AND,
                TOKEN_TYPE_LOGICAL_OR,
                TOKEN_TYPE_LOGICAL_NOT,
                // relational
                TOKEN_TYPE_EQUAL,
                TOKEN_TYPE_NOT_EQUAL,
                TOKEN_TYPE_LESS,
                TOKEN_TYPE_GREATER,
                TOKEN_TYPE_LESS_EQUAL,
                TOKEN_TYPE_GREATER_EQUAL,
                // invalid
                TOKEN_TYPE_INVALID};

std::string token2string (Token token);

#endif
