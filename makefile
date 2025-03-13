flags = -Wall -g
flags += -I include
exec_dir = bin
build_dir = build
test_dir = test
src_dir = src

lexer.o: $(src_dir)/lexer.c
	gcc $(flags) -c $< -o $(build_dir)/lexer.o

token.o: $(src_dir)/token.c
	gcc $(flags) -c $< -o $(build_dir)/token.o

lexer_test.o: $(test_dir)/lexer_test.c
	gcc $(flags) -c $< -o $(build_dir)/lexer_test.o 

token_test.o: $(test_dir)/token_test.c
	gcc $(flags) -c $< -o $(build_dir)/token_test.o

main.o: $(src_dir)/main.c
	gcc $(flags) -c $< -o $(build_dir)/main.o

main: main.o lexer.o token.o
	gcc $(flags) $(build_dir)/main.o $(build_dir)/lexer.o $(build_dir)/token.o -o $(exec_dir)/main

lexer_test: lexer_test.o lexer.o token.o
	gcc $(flags) $(build_dir)/lexer_test.o $(build_dir)/token.o $(build_dir)/lexer.o -o $(exec_dir)/lexer_test

token_test: token_test.o
	gcc $(flags) $(build_dir)/token_test.o -o $(exec_dir)/token_test

test: lexer_test token_test
	$(exec_dir)/lexer_test
	$(exec_dir)/token_test

clean:
	rm build/*.o bin/*

	