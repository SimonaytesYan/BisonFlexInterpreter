%option c++ yylineno
%option outfile="Lexer.cpp" header-file="Lexer.hpp"

%{
#include <cstring>

#include "AST.hpp"
#include "Logger.hpp"
#include "Parser.hpp"

using namespace yy;

%}

%%
"var"                   { log_out << "LEXER: keyword var!\n";   return parser::token::CREATE_VAR; }
"write"                 { log_out << "LEXER: keyword write!\n"; return parser::token::WRITELN;}
"read"                  { log_out << "LEXER: keyword read!\n";  return parser::token::READLN;}
"begin"                 { log_out << "LEXER: keyword begin!\n"; return parser::token::BEGIN_SCOPE;}
"end"                   { log_out << "LEXER: keyword end!\n";   return parser::token::END_SCOPE;}
"if"                    { log_out << "LEXER: keyword if!\n";    return parser::token::IF;}
"then"                  { log_out << "LEXER: keyword then!\n";  return parser::token::THEN;}
"else"                  { log_out << "LEXER: keyword if!\n";    return parser::token::ELSE;}
"while"                 { log_out << "LEXER: keyword while!\n"; return parser::token::WHILE;}
"do"                    { log_out << "LEXER: keyword do!\n";    return parser::token::DO;}
"for"                   { log_out << "LEXER: keyword for!\n";   return parser::token::FOR;}
"to"                    { log_out << "LEXER: keyword to!\n";    return parser::token::TO;}

"+"                     { log_out << "LEXER: +\n";              return parser::token::ADD; }
"-"                     { log_out << "LEXER: -\n";              return parser::token::SUB; }
"*"                     { log_out << "LEXER: *\n";              return parser::token::MUL; }
"/"                     { log_out << "LEXER: /\n";              return parser::token::DIV; }
"="                     { log_out << "LEXER: =\n";              return parser::token::IS_EQ; }
"<>"                    { log_out << "LEXER: <>\n";             return parser::token::IS_NOT_EQ; }
">="                    { log_out << "LEXER: >=\n";             return parser::token::IS_GE; }
"<="                    { log_out << "LEXER: <=\n";             return parser::token::IS_LE; }
">"                     { log_out << "LEXER: >\n";              return parser::token::IS_G; }
"<"                     { log_out << "LEXER: <\n";              return parser::token::IS_L; }
":="                    { log_out << "LEXER: =\n";              return parser::token::EQ; }

"and"                   { log_out << "LEXER: and\n";            return parser::token::AND; }
"or"                    { log_out << "LEXER: or\n";             return parser::token::OR; }
"not"                   { log_out << "LEXER: not\n";            return parser::token::NOT; }

"("                     { log_out << "LEXER: (\n";              return parser::token::LBRACKET; }
")"                     { log_out << "LEXER: )\n";              return parser::token::RBRACKET; }
";"                     { log_out << "LEXER: ;\n";              return parser::token::SEMICOLON; }
"."                     { log_out << "LEXER: .\n";              return parser::token::DOT; }
"\""                    { log_out << "LEXER: \"\n";             return parser::token::QUOTES; }

[0-9]+                  { log_out << "LEXER: NUM!\n";           return parser::token::NUM; }
[a-zA-Z_][a-zA-Z_0-9_]* { log_out << "LEXER: VAR!\n";           return parser::token::VAR; }

[ \r\n\t]*              // Nothing
.                       { log_out << "LEXER: It is ok!\n";      return parser::token::EMPTY; }

%% 

int yyFlexLexer::yywrap() { return 1; }