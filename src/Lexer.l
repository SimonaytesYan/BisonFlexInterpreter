%option c++ yylineno
%option outfile="Lexer.cpp" header-file="Lexer.hpp"

%{
#include <cstring>

#include "AST.hpp"
#include "Parser.hpp"

using namespace yy;

// [a-zA-Z][a-zA-Z0-9]* { sscanf(yytext, "%s", &yylval->value); return VAR; }
// [0-9]+               { std::cerr << "LEXER: number!\n"; return parser::token::NUM; }
// "+"                  { std::cerr << "LEXER: +\n"; return parser::token::ADD; }
// "-"                  { std::cerr << "LEXER: -\n"; return parser::token::SUB; }
// "*"                  { std::cerr << "LEXER: *\n"; return parser::token::MUL; }
// "/"                  { std::cerr << "LEXER: /\n"; return parser::token::DIV; }
// "("                  { std::cerr << "LEXER: (\n"; return parser::token::LBRACKET; }
// ")"                  { std::cerr << "LEXER: )\n"; return parser::token::RBRACKET; }
// [ \r\n\t]*           { std::cerr << "LEXER: space symbol\n"; return 0; } // Nothing
// .                    { std::cerr << "HUHUHoisdfiwdvpieh voisevopiesv-0iesvpvi\n" << lineno() << ": Unexpected character: '" << YYText() << "'." << std::endl; exit(1); }
// .                    { std::cerr << "HUHUHoisdfiwdvpieh voisevopiesv-0iesvpvi\n" << std::endl; exit(1); }
%}


%%
[0-9]+                  { std::cerr << "LEXER: NUM!\n";           return parser::token::NUM; }
"var"                   { std::cerr << "LEXER: keyword var!\n";   return parser::token::CREATE_VAR; }
"write"                 { std::cerr << "LEXER: keyword write!\n"; return parser::token::WRITELN;}
"read"                  { std::cerr << "LEXER: keyword read!\n";  return parser::token::READLN;}
[a-zA-Z_][a-zA-Z_0-9_]* { std::cerr << "LEXER: VAR!\n";           return parser::token::VAR; }
"+"                     { std::cerr << "LEXER: +\n";              return parser::token::ADD; }
"-"                     { std::cerr << "LEXER: -\n";              return parser::token::SUB; }
"*"                     { std::cerr << "LEXER: *\n";              return parser::token::MUL; }
"/"                     { std::cerr << "LEXER: /\n";              return parser::token::DIV; }
":="                    { std::cerr << "LEXER: =\n";              return parser::token::EQ; }
"("                     { std::cerr << "LEXER: (\n";              return parser::token::LBRACKET; }
")"                     { std::cerr << "LEXER: )\n";              return parser::token::RBRACKET; }
";"                     { std::cerr << "LEXER: ;\n";              return parser::token::SEMICOLON; }
[ \r\n\t]*              // Nothing
.                       { std::cerr << "LEXER: It is ok!\n";      return parser::token::EMPTY; }

%% 

int yyFlexLexer::yywrap() { return 1; }