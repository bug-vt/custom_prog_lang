%{
    #include <stdio.h>
    extern int yylex();
%}
%option noyywrap
%%
[ \t]               ; // ignore white space
\,                  { printf ("%s\t\tComma\n", yytext); }
\:                  { printf ("%s\t\tColon\n", yytext); }
\[                  { printf ("%s\t\tOpening bracket\n", yytext); }
\]                  { printf ("%s\t\tClosing bracket\n", yytext); }
\n                  { printf ("\\n\t\tLine break\n"); }
\{                  { printf ("%s\t\tOpening curly brace\n", yytext); }
\}                  { printf ("%s\t\tClosing curly brace\n", yytext); }
[0-9]+\.[0-9]+      { printf ("%s\t\tFloating-point\n", yytext); }
[0-9]+              { printf ("%s\t\tInteger\n", yytext); }
SetStackSize        { printf ("%s\t\tSetStackSize directive\n", yytext); }
var                 { printf ("%s\t\tvar directive\n", yytext); }
func                { printf ("%s\t\tfunc directive\n", yytext); }
param               { printf ("%s\t\tparam directive\n", yytext); }
mov|add|sub|mul|div|mod|exp|neg|inc|dec|and|or|xor|not|shl|shr|concat|getChar|setChar|jmp|je|jne|jg|jl|jge|jle|push|pop|call|ret|exit { printf ("%s\t\tInstruction\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_.]* { printf ("%s\t\tIdentifier\n", yytext); }
\".*\"              { printf ("%s\t\tString\n", yytext); }
.                   { printf ("Invalid\n"); exit (1); }
%%
int main ()
{
    // lex through the input
    while (yylex ());
}
