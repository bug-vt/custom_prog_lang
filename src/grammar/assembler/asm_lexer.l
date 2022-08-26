%{
    #include <stdio.h>
    extern int yylex();
%}
%option noyywrap
%%
;.*                 ; // ignore comment
[ \t]               ; // ignore white space
\,                  { printf ("%s\t\tComma\n", yytext); }
\:                  { printf ("%s\t\tColon\n", yytext); }
\[                  { printf ("%s\t\tOpening bracket\n", yytext); }
\]                  { printf ("%s\t\tClosing bracket\n", yytext); }
\n                  { printf ("\\n\t\tLine break\n"); }
[0-9]+\.[0-9]+      { printf ("%s\t\tFloating-point\n", yytext); }
[0-9]+              { printf ("%s\t\tInteger\n", yytext); }
setStackSize        { printf ("%s\t\tSetStackSize directive\n", yytext); }
var                 { printf ("%s\t\tvar directive\n", yytext); }
func                { printf ("%s\t\tfunc directive\n", yytext); }
_retVal             { printf ("%s\t\t_retVal register\n", yytext); }
mov|add|sub|mul|div|mod|exp|neg|inc|dec|and|or|xor|not|shl|shr|concat|getChar|setChar|jmp|je|jne|jg|jl|jge|jle|push|pop|call|ret|exit { printf ("%s\t\tInstruction\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_.]* { printf ("%s\t\tIdentifier\n", yytext); }
\".*\"              { printf ("%s\t\tString\n", yytext); }
.                   { printf ("Invalid\n"); exit (1); }
%%
int main (int argc, char **argv)
{
    if (argc != 2)
    {
        printf ("usage: asm_lexer <Source file>\n");
        return -1;
    }
    FILE *myfile = fopen (argv[1], "r");
    if (!myfile)
    {
        printf ("File not found: example.source");
        return -1;
    }
    yyin = myfile;

    // lex through the input
    while (yylex ());
}
