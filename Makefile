CC = g++
CFLAGS = -g -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

SRC = src
BUILD = build
BIN = bin

FILES = $(BUILD)/Lexer.cpp $(BUILD)/Parser.cpp $(BUILD)/AST.cpp $(BUILD)/Node.cpp $(BUILD)/main.cpp
HEADERS = $(BUILD)/AST.hpp $(BUILD)/Node.hpp

$(BIN)/test: $(FILES) $(HEADERS)
	$(CC) $(CFLAGS) $(FILES) -o $(BIN)/test

$(BUILD)/Lexer.cpp: $(SRC)/Lexer.l make_dir
	flex $(SRC)/Lexer.l
	mv Lexer.cpp $(BUILD)
	mv Lexer.hpp $(BUILD) 

$(BUILD)/Parser.cpp: $(SRC)/Parser.y $(BUILD)/Lexer.cpp make_dir
	bison -t $(SRC)/Parser.y
	mv location.hh $(BUILD)
	mv position.hh $(BUILD)
	mv stack.hh $(BUILD)
	mv Parser.cpp $(BUILD)
	mv Parser.hpp $(BUILD)

$(BUILD)/%.cpp: $(SRC)/%.cpp
	cp $< $@

$(BUILD)/%.hpp: $(SRC)/%.hpp
	cp $< $@

make_dir:
	-mkdir $(BUILD)
	-mkdir GraphicDumps
	-mkdir $(BIN)

clean:
	rm -r build
