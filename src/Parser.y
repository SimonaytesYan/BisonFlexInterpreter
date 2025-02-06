%language "c++"
%locations
%define api.value.type variant

%output  "Parser.cpp"
%defines "Parser.hpp"

%code top{
#include "AST.hpp"
#include "Logger.hpp"

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
%token FOR
%token TO
%token BEGIN_SCOPE "begin"
%token END_SCOPE   "end"

%left EQ IS_EQ IS_NOT_EQ IS_GE IS_LE IS_G IS_L

%left OR
%left AND
%left NOT

%left ADD SUB
%left MUL DIV 

%token LBRACKET  "("
%token RBRACKET  ")"
%token SEMICOLON ";"
%token DOT       "."
%token QUOTES

%token <std::string> NUM
%token <std::string> VAR

%token EMPTY

%type <Node*> expr_list
%type <Node*> expr
%type <Node*> logical_op
%type <Node*> compare_oper
%type <Node*> add_sub
%type <Node*> mul_div
%type <Node*> value
%type <Node*> get_var
%type <Node*> str
%type <std::string> char
%type <std::string> characters

%%

start: 
   "begin" expr_list "end" "." { AST ast($2); ast.graphicDump(); ast.run(); }
;

expr_list:
    expr    { $$ = $1; }
|
    expr expr_list  { $$ = new Node($1, $2); }

expr:
    CREATE_VAR get_var ";"           { log_out << "SYNTAX: create var \n"; 
                                       $$ = new Node(Keyword::VAR, $2, nullptr); }
|
    get_var EQ logical_op ";"        { log_out << "SYNTAX: var\n"; 
                                       $$ = new Node(Operator::EQUATE, $1, $3); }
|
    WRITELN "(" logical_op ")" ";"   { log_out << "SYNTAX: write \n"; 
                                       $$ = new Node(Keyword::WRITE, $3, nullptr); }
|
    WRITELN "(" str ")" ";"          { $$ = new Node(Keyword::WRITE, $3, nullptr); }
|
    READLN "(" get_var ")" ";"       { log_out << "SYNTAX: read \n"; 
                                       $$ = new Node(Keyword::READ, $3, nullptr); }
|
    IF logical_op THEN "begin" expr_list "end" ELSE "begin" expr_list "end" ";" { 
                                       Node* statement    = $2;
                                       Node* true_branch  = $5;
                                       Node* false_branch = $9;
                                       Node* branches = new Node($5, $9);
                                       $$ = new Node(Keyword::IF, statement, branches); }
|
    WHILE logical_op DO "begin" expr_list "end" ";" { 
                                       Node* statement  = $2;
                                       Node* cycle_body = $5;
                                       $$ = new Node(Keyword::WHILE, statement, cycle_body); }
|
    FOR get_var EQ logical_op TO logical_op DO "begin" expr_list "end" ";" { 
                                       Node* counter    = $2;
                                       Node* start_val  = $4;
                                       Node* finish_val = $6;
                                       Node* cycle_body = $9;

                                       Node* start_finish = new Node(start_val, finish_val);
                                       Node* statement = new Node(counter, start_finish);

                                       $$ = new Node(Keyword::FOR, statement, cycle_body); }
;

str:
    QUOTES characters QUOTES { $$ = Node::CreateString($2); }
;

characters:
    char { $$ = $1; }
|
    characters char { $$ = $1 + $2; }
;

char:
    VAR                   { $$ = std::string($1); }
|
    NUM                   { $$ = std::string($1); }
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
    add_sub ADD add_sub { log_out << "SYNTAX: add \n"; 
                          $$ = new Node(Operator::ADD, $1, $3); }
|
    add_sub SUB add_sub { log_out << "SYNTAX: sub \n"; 
                          $$ = new Node(Operator::SUB, $1, $3); }
|
    mul_div             { $$ = $1; }
;

mul_div:
    mul_div MUL mul_div { log_out << "SYNTAX: mul \n"; 
                          $$ = new Node(Operator::MUL, $1, $3); }
|
    mul_div DIV mul_div { log_out << "SYNTAX: div \n"; 
                          $$ = new Node(Operator::DIV, $1, $3); }
|
    value { $$ = $1; }
;

value:
    "(" expr ")" { $$ = $2; }
|
    NUM     { std::string num = $1; 
              log_out << "SYNTAX: NUM = " + num + "\n"; 
              $$ = new Node(atoi(num.c_str()), nullptr, nullptr); }
|
    get_var { $$ = $1; }
;

get_var:
    VAR { std::string var = $1; 
          log_out << "SYNTAX: VAR = " + $1 + "\n"; 
          $$ = new Node(var.c_str(), nullptr, nullptr); }
;

