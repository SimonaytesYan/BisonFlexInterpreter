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

// %token ADD
// %token SUB
// %token MUL
// %token DIV

// %token LBRACKET "("
// %token RBRACKET ")"

// %token <char> SYM
// %token <char*> VAR
// %token NUM
// %token ONE

// %type <Node*> expr

/* Precedence (increasing) and associativity:
   a+b+c is (a+b)+c: left associativity
   a+b*c is a+(b*c): the precedence of "*" is higher than that of "+". */

// %left ADD SUB
// %left MUL DIV
       // expr { CreateAST($1); RunAST(); }

// expr: 
// //     expr ADD expr { $$ = new Node(Operator::ADD, $1, $3); }
// // |
// //     expr SUB expr { $$ = new Node(Operator::SUB, $1, $3); }
// // |
// //     expr MUL expr { $$ = new Node(Operator::MUL, $1, $3); }
// // |
// //     expr DIV expr { $$ = new Node(Operator::DIV, $1, $3); }
// // |
// //     "(" expr ")"  { $$ = $2; }
// // |
//     // NUM           { $$ = new Node($1, nullptr, nullptr); }
//   ONE { std::cerr << "posidnfn-eunpiesespoesfoesnf[oesno[sefmsokfn[soef[oseofn[okwok;jlihlihlihikj]]]]]\n"`; $$ = new Node("1", nullptr, nullptr);}
// ;

%token <std::string> NUM

%token ADD
%token SUB
%token MUL
%token DIV
%token LBRACKET "("
%token RBRACKET ")"

%token EMPTY

%type <Node*> expr
%type <Node*> add_sub
%type <Node*> mul_div
%type <Node*> value

%%

start: 
    expr { AST ast($1); ast.graphicDump(); ast.run(); }
;

expr:
    add_sub { $$ = $1; }
;

add_sub:
    add_sub ADD add_sub { std::cerr << "SYNTAX: add \n"; $$ = new Node(Operator::ADD, $1, $3); }
|
    add_sub SUB add_sub { std::cerr << "SYNTAX: sub \n"; $$ = new Node(Operator::SUB, $1, $3); }
|
    mul_div             { $$ = $1; } 
;

mul_div:
    mul_div MUL mul_div { std::cerr << "SYNTAX: mul \n"; $$ = new Node(Operator::MUL, $1, $3); }
|
    mul_div DIV mul_div { std::cerr << "SYNTAX: div \n"; $$ = new Node(Operator::DIV, $1, $3); }
|
    value { $$ = $1; }
;

value:
    LBRACKET expr RBRACKET { $$ = $2; }
|
    NUM { std::cerr << "SYNTAX: NUM = " << $1 << "\n"; $$ = new Node($1, nullptr, nullptr); }
;
