FILES = Lexer.cpp Parser.cpp AST.cpp main.cpp
CC = g++
CFLAGS = -g -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

test: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o test

Lexer.cpp: Lexer.l
	flex Lexer.l

Parser.cpp: Parser.y Lexer.cpp
	bison -t Parser.y

clean:
	rm -f *.o *~ Lexer.cpp Lexer.hpp Parser.cpp Parser.hpp position.hh stack.hh location.hh test
