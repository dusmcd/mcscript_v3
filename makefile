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

list.o: $(src_dir)/list.c
	gcc $(flags) -c $< -o $(build_dir)/list.o

main.o: $(src_dir)/main.c
	gcc $(flags) -c $< -o $(build_dir)/main.o

parser.o: $(src_dir)/parser.c
	gcc $(flags) -c $< -o $(build_dir)/parser.o

memory_mgr.o: $(src_dir)/memory_mgr.c
	gcc $(flags) -c $< -o $(build_dir)/memory_mgr.o

main: main.o lexer.o token.o
	gcc $(flags) $(build_dir)/main.o $(build_dir)/lexer.o $(build_dir)/token.o -o $(exec_dir)/main

parser_test.o: $(test_dir)/parser_test.c
	gcc $(flags) -c $< -o $(build_dir)/parser_test.o

lexer_test: lexer_test.o lexer.o token.o
	gcc $(flags) $(build_dir)/lexer_test.o $(build_dir)/token.o $(build_dir)/lexer.o -o $(exec_dir)/lexer_test

parser_test: parser_test.o lexer.o parser.o token.o memory_mgr.o list.o
	gcc $(flags) $(build_dir)/parser_test.o $(build_dir)/lexer.o $(build_dir)/parser.o $(build_dir)/token.o $(build_dir)/memory_mgr.o \
		$(build_dir)/list.o -o $(exec_dir)/parser_test

test: lexer_test parser_test
	$(exec_dir)/lexer_test
	$(exec_dir)/parser_test

clean:
	rm build/*.o bin/*

	