%language "c++"
%locations
%define api.value.type variant

%output  "Parser.cpp"
%defines "Parser.hpp"

%code top{
#include "AST.hpp"
#include <string>
}


%code provides {
int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc);
}

%token CREATE_VAR
%token WRITELN
%token READLN

%token ADD
%token SUB
%token MUL
%token DIV
%token EQ

%token LBRACKET "("
%token RBRACKET ")"
%token SEMICOLON ";"

%token <std::string> NUM
%token <std::string> VAR

%token EMPTY

%type <Node*> expr
%type <Node*> add_sub
%type <Node*> mul_div
%type <Node*> value
%type <Node*> get_var

%%

start: 
    expr { AST ast($1); ast.graphicDump(); ast.run(); }
;

expr:
    CREATE_VAR get_var ";"      { std::cerr << "SYNTAX: create var \n"; 
                                  $$ = new Node(Keyword::VAR, $2, nullptr); }
|
    get_var EQ add_sub ";"      { std::cerr << "SYNTAX: var\n"; 
                                  $$ = new Node(Operator::EQUATE, $1, $3); }
|
    WRITELN "(" add_sub ")" ";" { std::cerr << "SYNTAX: write \n"; 
                                  $$ = new Node(Keyword::WRITE, $3, nullptr); }
|
    READLN "(" get_var ")" ";"  { std::cerr << "SYNTAX: read \n"; 
                                  $$ = new Node(Keyword::READ, $3, nullptr); }
|
    expr expr                   { $$ = new Node($1, $2); }
;

add_sub:
    add_sub ADD add_sub { std::cerr << "SYNTAX: add \n"; 
                          $$ = new Node(Operator::ADD, $1, $3); }
|
    add_sub SUB add_sub { std::cerr << "SYNTAX: sub \n"; 
                          $$ = new Node(Operator::SUB, $1, $3); }
|
    mul_div             { $$ = $1; }
;

mul_div:
    mul_div MUL mul_div { std::cerr << "SYNTAX: mul \n"; 
                          $$ = new Node(Operator::MUL, $1, $3); }
|
    mul_div DIV mul_div { std::cerr << "SYNTAX: div \n"; 
                          $$ = new Node(Operator::DIV, $1, $3); }
|
    value { $$ = $1; }
;

value:
    "(" expr ")" { $$ = $2; }
|
    NUM     { std::string num = $1; 
              std::cerr << "SYNTAX: NUM = " + num + "\n"; 
              $$ = new Node(atoi(num.c_str()), nullptr, nullptr); }
|
    get_var { $$ = $1; }
;

get_var:
    VAR { std::string var = $1; 
          std::cerr << "SYNTAX: VAR = " + $1 + "\n"; 
          $$ = new Node(var.c_str(), nullptr, nullptr); }
;

