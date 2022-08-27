%{
    #include <stdio.h>
    #include "asm_parser.tab.h"
%}
%%
;.*                 ; // ignore comment
[ \t]               ; // ignore white space
\,                  { yylval.comma = strdup(yytext); return COMMA; }
\:                  { yylval.colon = strdup(yytext); return COLON; }
\[                  { yylval.open_bracket = strdup(yytext); return OPEN_BRACKET; }
\]                  { yylval.close_bracket = strdup(yytext); return CLOSE_BRACKET; }
\n                  { yylval.new_line = strdup(yytext); return NEW_LINE; }
[0-9]+\.[0-9]+      { yylval.floating_point = atof(yytext); return FLOAT; }
[0-9]+              { yylval.integer = atoi(yytext); return INT; }
setStackSize        { yylval.set_stack_size = strdup(yytext); return SET_STACK_SIZE; }
var                 { yylval.var = strdup(yytext); return VAR; }
func                { yylval.func = strdup(yytext); return FUNC; }
_retVal             { yylval.return_val = strdup(yytext); return RET_VAL; }
push                { yylval.push = strdup(yytext); return PUSH; }
call                { yylval.call = strdup(yytext); return CALL; }
jmp                 { yylval.jump = strdup(yytext); return JUMP; }
ret                 { yylval.inst0 = strdup(yytext); return INST0; } 
neg|inc|dec|not|pop { yylval.inst1 = strdup(yytext); return INST1; }
mov|add|sub|mul|div|mod|exp|and|or|xor|shl|shr { yylval.inst2 = strdup(yytext); return INST2; }
je|jne|jg|jl|jge|jle { yylval.inst3 = strdup(yytext); return INST3; }
concat              { yylval.concat = strdup(yytext); return CONCAT; }
getChar             { yylval.getChar = strdup(yytext); return GETCHAR; }
setChar             { yylval.setChar = strdup(yytext); return SETCHAR; }
[a-zA-Z_][a-zA-Z0-9_.]* { yylval.ident = strdup(yytext); return IDENT; }
\".*\"              { yylval.string = strdup(yytext); return STRING; }
.                   ; 
%%
