flags = -Wall -g
flags += -I include
exec_dir = bin
build_dir = build
test_dir = test
include_dir = include
src_dir = src

lexer.o: $(src_dir)/lexer.c
	gcc $(flags) -c $< -o $(build_dir)/lexer.o

token.o: $(src_dir)/token.c
	gcc $(flags) -c $< -o $(build_dir)/token.o

lexer_test.o: $(test_dir)/lexer_test.c
	gcc $(flags) -c $< -o $(build_dir)/lexer_test.o 

lexer_test: lexer_test.o lexer.o token.o
	gcc $(flags) $(build_dir)/lexer_test.o $(build_dir)/token.o $(build_dir)/lexer.o -o $(exec_dir)/lexer_test

test: lexer_test
	$(exec_dir)/lexer_test

clean:
	rm build/*.o bin/*

	