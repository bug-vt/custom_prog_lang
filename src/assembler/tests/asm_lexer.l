%{
    #include <iostream>
    #include <cstdio>
    #include <string.h>
    #include "asm_parser.tab.h"
    using namespace std;
    int line_num = 1;
    string lex_output = "";
%}
%%
;.*                 ; // ignore comment
[ \t]               ; // ignore white space
\,                  { lex_output += string (yytext) + "\t\tcomma\n"; return COMMA; }
\:                  { lex_output += string (yytext) + "\t\tcolon\n"; return COLON; }
\[                  { lex_output += string (yytext) + "\t\topen bracket\n"; return OPEN_BRACKET; }
\]                  { lex_output += string (yytext) + "\t\tclose bracket\n"; return CLOSE_BRACKET; }
\{                  { lex_output += string (yytext) + "\t\topen brace\n"; return OPEN_BRACE; }
\}                  { lex_output += string (yytext) + "\t\tclose brace\n"; return CLOSE_BRACE; }
\n                  { lex_output += "\\n\t\tnew line\n"; line_num++; return NEW_LINE; }
[0-9]+\.[0-9]+      { lex_output += string (yytext) + "\t\tfloat\n"; yylval.floating_point = atof(yytext); return FLOAT; }
[0-9]+              { lex_output += string (yytext) + "\t\tint\n"; yylval.integer = atoi(yytext); return INT; }
setStackSize        { lex_output += string (yytext) + "\t\tsetStackSize\n"; return SET_STACK_SIZE; }
param               { lex_output += string (yytext) + "\t\tparam\n"; return PARAM; }
var                 { lex_output += string (yytext) + "\t\tvar\n"; return VAR; }
func                { lex_output += string (yytext) + "\t\tfunc\n"; return FUNC; }
_retVal             { lex_output += string (yytext) + "\t\t_retVal register\n"; return RET_VAL; }
push                { lex_output += string (yytext) + "\t\tinstruction\n"; return PUSH; }
call                { lex_output += string (yytext) + "\t\tinstruction\n"; return CALL; }
jmp                 { lex_output += string (yytext) + "\t\tinstruction\n"; return JUMP; }
ret                 { lex_output += string (yytext) + "\t\tinstruction\n"; yylval.inst0 = strdup(yytext); return INST0; } 
neg|inc|dec|not|pop { lex_output += string (yytext) + "\t\tinstruction\n"; yylval.inst1 = strdup(yytext); return INST1; }
mov|add|sub|mul|div|mod|exp|and|or|xor|shl|shr { lex_output += string (yytext) + "\t\tinstruction\n"; yylval.inst2 = strdup(yytext); return INST2; }
je|jne|jg|jl|jge|jle { lex_output += string (yytext) + "\t\tinstruction\n"; yylval.inst3 = strdup(yytext); return INST3; }
concat              { lex_output += string (yytext) + "\t\tinstruction\n"; return CONCAT; }
getChar             { lex_output += string (yytext) + "\t\tinstruction\n"; return GETCHAR; }
setChar             { lex_output += string (yytext) + "\t\tinstruction\n"; return SETCHAR; }
[a-zA-Z_][a-zA-Z0-9_.]* { lex_output += string (yytext) + "\t\tidentifier\n"; yylval.ident = strdup(yytext); return IDENT; }
\".*\"              { lex_output += string (yytext) + "\t\tstring\n"; yylval.string = strdup(yytext); return STRING; }
.                   { lex_output += string (yytext) + "\t\tinvlaid\n"; return INVALID; }
%%
