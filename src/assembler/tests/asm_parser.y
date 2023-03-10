%{
    #include <iostream>
    #include <cstdio>
    #include <cstdlib>
    using namespace std;
    // stuff that need from flex
    extern int yylex ();
    extern int yyparse ();
    extern FILE *yyin;
    extern int line_num;
    extern string lex_output;

    void yyerror (const char *s);
%}

// overwirte default yystype from int to union.
%union 
{
    float floating_point;
    int integer;
    char *ident;
    char *string;
    char *inst0;
    char *inst1;
    char *inst2;
    char *inst3;
}

// constant-string tokens
%token COMMA
%token COLON
%token OPEN_BRACKET
%token CLOSE_BRACKET
%token OPEN_BRACE
%token CLOSE_BRACE
%token NEW_LINE
%token SET_STACK_SIZE
%token PARAM
%token VAR
%token FUNC
%token RET_VAL
%token PUSH
%token CALL
%token JUMP
%token CONCAT
%token GETCHAR
%token SETCHAR
%token INVALID

// terminal symbol token types.
%token <floating_point> FLOAT
%token <integer> INT
%token <ident> IDENT
%token <string> STRING
%token <inst0> INST0
%token <inst1> INST1
%token <inst2> INST2
%token <inst3> INST3

%%
// grammar
assembly:
    assembly line
    | line
    ;

line:
    instruction3 NEW_LINE
    | instruction2 NEW_LINE
    | instruction1 NEW_LINE
    | INST0 NEW_LINE
    | JUMP IDENT NEW_LINE
    | CONCAT STRING COMMA STRING NEW_LINE
    | GETCHAR destination COMMA source COMMA INT NEW_LINE
    | SETCHAR INT destination COMMA source COMMA NEW_LINE
    | PUSH source NEW_LINE
    | CALL IDENT NEW_LINE
    | SET_STACK_SIZE INT NEW_LINE
    | PARAM identifier NEW_LINE
    | VAR identifier NEW_LINE
    | FUNC IDENT NEW_LINE OPEN_BRACE
    | label
    | NEW_LINE
    | CLOSE_BRACE
    ;

instruction3:
    INST3 op COMMA op COMMA IDENT
    ;

instruction2:
    INST2 destination COMMA source
    ;

instruction1:
    INST1 destination
    ;

destination:
    IDENT OPEN_BRACKET INT CLOSE_BRACKET
    | IDENT
    | RET_VAL
    ;
source:
    IDENT OPEN_BRACKET INT CLOSE_BRACKET
    | IDENT
    | INT
    | FLOAT
    | STRING
    | RET_VAL
    ;

op:
    source
    ;

label:
    IDENT COLON NEW_LINE
    | IDENT COLON
    ;

identifier:
    IDENT OPEN_BRACKET INT CLOSE_BRACKET 
    | IDENT 
    ;

%%
int main (int argc, char **argv)
{
    bool run_parser = false;
    if (argc != 3)
    {
        cout << "usage: asm_lexer option assembly_file" << endl
             << "\toption: -l lexer" << endl
             << "\t        -p parser" << endl;
        return -1;
    }
    if (string (argv[1]) == "-p")
        run_parser = true;

    FILE *myfile = fopen (argv[2], "r");
    if (!myfile)
    {
        cout << "File not found: " << argv[1] << endl;
        return -1;
    }
    // set flex to read from specified file instead of STDIN.
    yyin = myfile;

    if (run_parser)
        // parse through the input
        yyparse ();
    else
    {
        while (yylex ());
        cout << lex_output;
    }
}

void yyerror (const char *s)
{
    printf ("parse error on line %d: %s\n", line_num, s);
    exit (-1);
}
