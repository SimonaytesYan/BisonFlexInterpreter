FILES = Lexer.cpp Parser.cpp AST.cpp main.cpp
CC = g++
CFLAGS = -g -std=c++2a

test: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o test

Lexer.cpp: Lexer.l
	flex Lexer.l

Parser.cpp: Parser.y Lexer.cpp
	bison -t Parser.y

clean:
	rm -f *.o *~ Lexer.cpp Lexer.hpp Parser.cpp Parser.hpp position.hh stack.hh location.hh test
