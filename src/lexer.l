
%{
#include <stdio.h>

void number();
void variable();
void symbol();

%}

%%
[a-zA-Z][a-zA-Z0-9]* { printf("hui\n"); }
[0-9]+               { printf("pizda\n"); }
.                    { printf("ebat`\n"); }
\n                   { return 0; }
%%

int yywrap(){ return 0; }

// void number() { std::cout << "Number " << yytext << "\n"; }
// void variable() { std::cout << "Variable " << yytext << "\n"; }
// void symbol() { std::cout << "symbol " << yytext << "\n"; }

//  %option c++ noyywrap yylineno

int main() {
    const char* filename = "lex_test.txt";
    FILE *fp = fopen(filename,"r");

    yyin = fp;
    yylex();
    
    printf("\n\n\nБЛЯТЬ\n\n\n");
}
