%option c++ yylineno
%option outfile="Lexer.cpp" header-file="Lexer.hpp"

%{
#include <cstring>

#include "AST.hpp"
#include "Parser.hpp"

using namespace yy;

%}

%%
"var"                   { std::cerr << "LEXER: keyword var!\n";   return parser::token::CREATE_VAR; }
"write"                 { std::cerr << "LEXER: keyword write!\n"; return parser::token::WRITELN;}
"read"                  { std::cerr << "LEXER: keyword read!\n";  return parser::token::READLN;}
"begin"                 { std::cerr << "LEXER: keyword begin!\n"; return parser::token::BEGIN_SCOPE;}
"end"                   { std::cerr << "LEXER: keyword end!\n";   return parser::token::END_SCOPE;}
"if"                    { std::cerr << "LEXER: keyword if!\n";    return parser::token::IF;}
"then"                  { std::cerr << "LEXER: keyword then!\n";  return parser::token::THEN;}
"else"                  { std::cerr << "LEXER: keyword if!\n";    return parser::token::ELSE;}
"while"                 { std::cerr << "LEXER: keyword while!\n"; return parser::token::WHILE;}
"do"                    { std::cerr << "LEXER: keyword do!\n";    return parser::token::DO;}
"for"                   { std::cerr << "LEXER: keyword for!\n";   return parser::token::FOR;}
"to"                    { std::cerr << "LEXER: keyword to!\n";    return parser::token::TO;}

"+"                     { std::cerr << "LEXER: +\n";              return parser::token::ADD; }
"-"                     { std::cerr << "LEXER: -\n";              return parser::token::SUB; }
"*"                     { std::cerr << "LEXER: *\n";              return parser::token::MUL; }
"/"                     { std::cerr << "LEXER: /\n";              return parser::token::DIV; }
"="                     { std::cerr << "LEXER: =\n";              return parser::token::IS_EQ; }
"<>"                    { std::cerr << "LEXER: <>\n";             return parser::token::IS_NOT_EQ; }
">="                    { std::cerr << "LEXER: >=\n";             return parser::token::IS_GE; }
"<="                    { std::cerr << "LEXER: <=\n";             return parser::token::IS_LE; }
">"                     { std::cerr << "LEXER: >\n";              return parser::token::IS_G; }
"<"                     { std::cerr << "LEXER: <\n";              return parser::token::IS_L; }
":="                    { std::cerr << "LEXER: =\n";              return parser::token::EQ; }

"and"                   { std::cerr << "LEXER: and\n";            return parser::token::AND; }
"or"                    { std::cerr << "LEXER: or\n";             return parser::token::OR; }
"not"                   { std::cerr << "LEXER: not\n";            return parser::token::NOT; }

"("                     { std::cerr << "LEXER: (\n";              return parser::token::LBRACKET; }
")"                     { std::cerr << "LEXER: )\n";              return parser::token::RBRACKET; }
";"                     { std::cerr << "LEXER: ;\n";              return parser::token::SEMICOLON; }
"."                     { std::cerr << "LEXER: .\n";              return parser::token::DOT; }

[0-9]+                  { std::cerr << "LEXER: NUM!\n";           return parser::token::NUM; }
[a-zA-Z_][a-zA-Z_0-9_]* { std::cerr << "LEXER: VAR!\n";           return parser::token::VAR; }

[ \r\n\t]*              // Nothing
.                       { std::cerr << "LEXER: It is ok!\n";      return parser::token::EMPTY; }

%% 

int yyFlexLexer::yywrap() { return 1; }