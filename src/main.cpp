
#include "AST.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

#include <iostream>
#include <fstream>

const char* kTextFile = "lex_test.txt";

yyFlexLexer *lexer;

int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc) {
    yylloc->begin.line = lexer->lineno();
    int token = lexer->yylex();

    if(token == yy::parser::token::NUM || token == yy::parser::token::VAR) {
        yylval->build(std::string(lexer->YYText()));
    }

    return token;
}

void yy::parser::error(const location_type& loc, const std::string& msg) {
    std::cerr << "file:  " << loc.begin.filename << std::endl;
    std::cerr << "column " << loc.begin.column << std::endl;
    std::cerr << "Line " << loc.begin.line << ": " << msg << std::endl;
    exit(1);
}

int main(int argc, const char* argv[])
{
    if (argc != 2) {
        std::cerr << "Wrong number of arguments!\n";
        return -1; 
    }

    const char* code_file = argv[1];

    std::ifstream input(code_file);
    if (!input) {
        std::cerr << "Cannot open file " << code_file << "!\n";
        return -1; 
    }
    
    lexer = new yyFlexLexer(&input);
    yy::parser parser;
    // parser.set_debug_level(1);
    parser.parse();
    
    delete lexer;
}