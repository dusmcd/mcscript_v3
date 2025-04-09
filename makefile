flags = -Wall -g -std=c++17
flags += -I include -I test/include
exec_dir = bin
build_dir = build
test_dir = test/src
src_dir = src

# Source files

lexer.o: $(src_dir)/lexer.cc
	g++ $(flags) -c $< -o $(build_dir)/lexer.o

token.o: $(src_dir)/token.cc
	g++ $(flags) -c $< -o $(build_dir)/token.o


main.o: $(src_dir)/main.cc
	g++ $(flags) -c $< -o $(build_dir)/main.o

parser.o: $(src_dir)/parser.cc
	g++ $(flags) -c $< -o $(build_dir)/parser.o

ast.o: $(src_dir)/ast.cc
	g++ $(flags) -c $< -o $(build_dir)/ast.o

evaluator.o: $(src_dir)/evaluator.cc
	g++ $(flags) -c $< -o $(build_dir)/evaluator.o

# Test files

parser_test.o: $(test_dir)/parser_test.cc
	g++ $(flags) -c $< -o $(build_dir)/parser_test.o

lexer_test.o: $(test_dir)/lexer_test.cc
	g++ $(flags) -c $< -o $(build_dir)/lexer_test.o 

evaluator_test.o: $(test_dir)/evaluator_test.cc
	g++ $(flags) -c $< -o $(build_dir)/evaluator_test.o

# Executables

main: main.o lexer.o token.o parser.o ast.o evaluator.o
	g++ $(flags) $(build_dir)/main.o $(build_dir)/lexer.o $(build_dir)/token.o \
	$(build_dir)/parser.o $(build_dir)/ast.o $(build_dir)/evaluator.o -o $(exec_dir)/main


lexer_test: lexer_test.o lexer.o token.o
	g++ $(flags) $(build_dir)/lexer_test.o $(build_dir)/token.o $(build_dir)/lexer.o \
	-o $(exec_dir)/lexer_test

parser_test: parser_test.o lexer.o parser.o token.o ast.o
	g++ $(flags) $(build_dir)/parser_test.o $(build_dir)/lexer.o $(build_dir)/parser.o \
	$(build_dir)/token.o $(build_dir)/ast.o -o $(exec_dir)/parser_test

evaluator_test: evaluator_test.o lexer.o parser.o token.o ast.o evaluator.o
	g++ $(flags) $(build_dir)/evaluator_test.o $(build_dir)/lexer.o $(build_dir)/parser.o \
	$(build_dir)/token.o $(build_dir)/ast.o $(build_dir)/evaluator.o -o $(exec_dir)/evaluator_test


test: lexer_test parser_test evaluator_test
	$(exec_dir)/lexer_test
	$(exec_dir)/parser_test
	$(exec_dir)/evaluator_test

# Utility

clean:
	rm build/*.o bin/*

	