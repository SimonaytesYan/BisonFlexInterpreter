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

%token EMPTY

%type <Node*> expr

%%

start: 
    expr { AST ast($1); ast.graphicDump(); ast.run(); }
    // NUM { std::cerr << "SYNTAX: NUM = " << $1 << "\n";  }
;

expr:
    expr ADD expr { $$ = new Node(Operator::ADD, $1, $3); }
|
    expr SUB expr { $$ = new Node(Operator::SUB, $1, $3); }
|
    expr MUL expr { $$ = new Node(Operator::MUL, $1, $3); }
|
    expr DIV expr { $$ = new Node(Operator::DIV, $1, $3); }
|
    NUM { std::cerr << "SYNTAX: NUM = " << $1 << "\n"; $$ = new Node($1, nullptr, nullptr); }
;