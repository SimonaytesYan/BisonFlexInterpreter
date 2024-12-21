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
[0-9]+                  { std::cerr << "LEXER: NUM!\n"; return parser::token::NUM; }
"+"                     { std::cerr << "LEXER: +\n"; return parser::token::ADD; }
"-"                     { std::cerr << "LEXER: -\n"; return parser::token::SUB; }
"*"                     { std::cerr << "LEXER: *\n"; return parser::token::MUL; }
"/"                     { std::cerr << "LEXER: /\n"; return parser::token::DIV; }
.                       { std::cerr << "LEXER: It is ok!\n"; return parser::token::EMPTY; }

%% 

int yyFlexLexer::yywrap() { return 1; }