%{
    #include <stdio.h>
    #include <stdlib.h>
    extern int yylex ();
    extern int yyparse ();
    extern FILE *yyin;

    void yyerror (const char *s);
%}

%union 
{
    char *comma;
    char *colon;
    char *open_bracket;
    char *close_bracket;
    char *new_line;
    float floating_point;
    int integer;
    char *set_stack_size;
    char *var;
    char *func;
    char *return_val;
    char *push;
    char *call;
    char *jump;
    char *inst0;
    char *inst1;
    char *inst2;
    char *inst3;
    char *concat;
    char *getChar;
    char *setChar;
    char *ident;
    char *string;
}

%token <comma> COMMA
%token <colon> COLON
%token <open_bracket> OPEN_BRACKET
%token <close_bracket> CLOSE_BRACKET
%token <new_line> NEW_LINE
%token <floating_point> FLOAT
%token <integer> INT
%token <set_stack_size> SET_STACK_SIZE
%token <var> VAR
%token <func> FUNC
%token <return_val> RET_VAL
%token <push> PUSH
%token <call> CALL
%token <jump> JUMP
%token <inst0> INST0
%token <inst1> INST1
%token <inst2> INST2
%token <inst3> INST3
%token <concat> CONCAT
%token <getChar> GETCHAR
%token <setChar> SETCHAR
%token <ident> IDENT
%token <string> STRING

%%
line:
    instruction3 NEW_LINE { }
    | instruction2 NEW_LINE { }
    | instruction1 NEW_LINE { }
    | INST0 NEW_LINE { }
    | JUMP IDENT NEW_LINE { }
    | CONCAT STRING COMMA STRING NEW_LINE { }
    | GETCHAR destination COMMA source COMMA INT NEW_LINE { }
    | SETCHAR INT destination COMMA source COMMA NEW_LINE { }
    | PUSH source NEW_LINE { }
    | CALL IDENT NEW_LINE { }
    | SET_STACK_SIZE INT NEW_LINE { }
    | VAR identifier NEW_LINE { }
    | FUNC IDENT NEW_LINE { }
    | label { }
    ;

instruction3:
    INST3 op COMMA op COMMA IDENT { }
    ;

instruction2:
    INST2 destination COMMA source { }
    ;

instruction1:
    INST1 destination { }
    ;

destination:
    IDENT OPEN_BRACKET INT CLOSE_BRACKET { }
    | IDENT { }
    ;
source:
    IDENT OPEN_BRACKET INT CLOSE_BRACKET { }
    | IDENT { }
    | INT { }
    | FLOAT { }
    | STRING { }
    | RET_VAL { }
    ;

op:
    source { }
    ;

label:
    IDENT COLON { }
    ;

identifier:
    IDENT OPEN_BRACKET INT CLOSE_BRACKET { }
    | IDENT { }
    ;

%%
int main (int argc, char **argv)
{
    if (argc != 2)
    {
        printf ("usage: asm_lexer <assembly file>\n");
        return -1;
    }
    FILE *myfile = fopen (argv[1], "r");
    if (!myfile)
    {
        printf ("File not found: %s\n", argv[1]);
        return -1;
    }
    yyin = myfile;
    yyparse ();
}

void yyerror (const char *s)
{
    printf ("parse error: %s\n", s);
    exit (-1);
}
