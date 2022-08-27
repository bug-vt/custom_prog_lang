%{
    #include <stdio.h>
    #include "asm_parser.tab.h"
    int line_num = 1;
%}
%%
;.*                 ; // ignore comment
[ \t]               ; // ignore white space
\,                  { return COMMA; }
\:                  { return COLON; }
\[                  { return OPEN_BRACKET; }
\]                  { return CLOSE_BRACKET; }
\n                  { line_num++; return NEW_LINE; }
[0-9]+\.[0-9]+      { yylval.floating_point = atof(yytext); return FLOAT; }
[0-9]+              { yylval.integer = atoi(yytext); return INT; }
setStackSize        { return SET_STACK_SIZE; }
var                 { return VAR; }
func                { return FUNC; }
_retVal             { return RET_VAL; }
push                { return PUSH; }
call                { return CALL; }
jmp                 { return JUMP; }
ret                 { yylval.inst0 = strdup(yytext); return INST0; } 
neg|inc|dec|not|pop { yylval.inst1 = strdup(yytext); return INST1; }
mov|add|sub|mul|div|mod|exp|and|or|xor|shl|shr { yylval.inst2 = strdup(yytext); return INST2; }
je|jne|jg|jl|jge|jle { yylval.inst3 = strdup(yytext); return INST3; }
concat              { return CONCAT; }
getChar             { return GETCHAR; }
setChar             { return SETCHAR; }
[a-zA-Z_][a-zA-Z0-9_.]* { yylval.ident = strdup(yytext); return IDENT; }
\".*\"              { yylval.string = strdup(yytext); return STRING; }
.                   { return INVALID; }
%%
