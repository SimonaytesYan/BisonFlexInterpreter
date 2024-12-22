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
%token IF
%token THEN
%token ELSE
%token WHILE
%token DO
%token BEGIN_SCOPE "begin"
%token END_SCOPE   "end"

%token ADD
%token SUB
%token MUL
%token DIV

%token IS_EQ
%token IS_NOT_EQ
%token IS_GE
%token IS_LE
%token IS_G
%token IS_L
%token EQ

%token AND
%token OR
%token NOT

%token LBRACKET  "("
%token RBRACKET  ")"
%token SEMICOLON ";"
%token DOT       "."

%token <std::string> NUM
%token <std::string> VAR

%token EMPTY

%type <Node*> expr
%type <Node*> logical_op
%type <Node*> compare_oper
%type <Node*> add_sub
%type <Node*> mul_div
%type <Node*> value
%type <Node*> get_var

%%

start: 
   "begin" expr "end" "." { AST ast($2); ast.graphicDump(); ast.run(); }
;

expr:
    CREATE_VAR get_var ";"           { std::cerr << "SYNTAX: create var \n"; 
                                       $$ = new Node(Keyword::VAR, $2, nullptr); }
|
    get_var EQ logical_op ";"        { std::cerr << "SYNTAX: var\n"; 
                                       $$ = new Node(Operator::EQUATE, $1, $3); }
|
    WRITELN "(" logical_op ")" ";"   { std::cerr << "SYNTAX: write \n"; 
                                       $$ = new Node(Keyword::WRITE, $3, nullptr); }
|
    READLN "(" get_var ")" ";"       { std::cerr << "SYNTAX: read \n"; 
                                       $$ = new Node(Keyword::READ, $3, nullptr); }
|
    IF logical_op THEN "begin" expr "end" ELSE "begin" expr "end" ";" { 
                                       Node* statement    = $2;
                                       Node* true_branch  = $5;
                                       Node* false_branch = $9;
                                       Node* branches = new Node($5, $9);
                                       $$ = new Node(Keyword::IF, statement, branches); }
|
    WHILE logical_op DO "begin" expr "end" ";" { 
                                       Node* statement  = $2;
                                       Node* cycle_body = $5;
                                       $$ = new Node(Keyword::WHILE, statement, cycle_body); }
|
    expr expr                            { $$ = new Node($1, $2); }
;

logical_op:
    logical_op AND logical_op { $$ = new Node(Operator::AND, $1, $3); }
|
    logical_op OR  logical_op { $$ = new Node(Operator::OR,  $1, $3); }
|
    NOT logical_op            { $$ = new Node(Operator::NOT, $2, nullptr); }
|
    compare_oper              { $$ = $1; }
;

compare_oper:
    compare_oper IS_EQ     compare_oper { $$ = new Node(Operator::IS_EQ,     $1, $3); }
|
    compare_oper IS_NOT_EQ compare_oper { $$ = new Node(Operator::IS_NOT_EQ, $1, $3); }
|
    compare_oper IS_GE     compare_oper { $$ = new Node(Operator::IS_GE,     $1, $3); }
|
    compare_oper IS_LE     compare_oper { $$ = new Node(Operator::IS_LE,     $1, $3); }
|
    compare_oper IS_G      compare_oper { $$ = new Node(Operator::IS_G,      $1, $3); }
|
    compare_oper IS_L      compare_oper { $$ = new Node(Operator::IS_L,      $1, $3); }
|
    add_sub                             { $$ = $1; }
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

