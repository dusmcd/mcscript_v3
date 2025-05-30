flags = -Wall -g -std=c++17
flags += -I include -I test/include
exec_dir = bin
build_dir = build
test_dir = test/src
src_dir = src
eval_dep = evaluator_test.o lexer.o parser.o token.o\
 					ast.o evaluator.o gcollector.o object.o environment.o
builtin_dep = builtin_test.o lexer.o parser.o token.o\
 					ast.o evaluator.o gcollector.o object.o environment.o



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

gcollector.o: $(src_dir)/gcollector.cc
	g++ $(flags) -c $< -o $(build_dir)/gcollector.o

object.o: $(src_dir)/object.cc
	g++ $(flags) -c $< -o $(build_dir)/object.o

environment.o: $(src_dir)/environment.cc
	g++ $(flags) -c $< -o $(build_dir)/environment.o

# Test files

parser_test.o: $(test_dir)/parser_test.cc
	g++ $(flags) -c $< -o $(build_dir)/parser_test.o

lexer_test.o: $(test_dir)/lexer_test.cc
	g++ $(flags) -c $< -o $(build_dir)/lexer_test.o 

evaluator_test.o: $(test_dir)/evaluator_test.cc
	g++ $(flags) -c $< -o $(build_dir)/evaluator_test.o

builtin_test.o: $(test_dir)/builtin_test.cc
	g++ $(flags) -c $< -o $(build_dir)/builtin_test.o

# Executables

main: build/ bin/ main.o lexer.o token.o parser.o ast.o evaluator.o gcollector.o environment.o object.o
	g++ $(flags) $(build_dir)/main.o $(build_dir)/lexer.o $(build_dir)/token.o \
	$(build_dir)/parser.o $(build_dir)/ast.o $(build_dir)/evaluator.o $(build_dir)/gcollector.o \
	$(build_dir)/object.o $(build_dir)/environment.o -o $(exec_dir)/main


lexer_test: build/ bin/ lexer_test.o lexer.o token.o
	g++ $(flags) $(build_dir)/lexer_test.o $(build_dir)/token.o $(build_dir)/lexer.o \
	-o $(exec_dir)/lexer_test

parser_test: build/ bin/ parser_test.o lexer.o parser.o token.o ast.o
	g++ $(flags) $(build_dir)/parser_test.o $(build_dir)/lexer.o $(build_dir)/parser.o \
	$(build_dir)/token.o $(build_dir)/ast.o -o $(exec_dir)/parser_test

evaluator_test: build/ bin/ $(eval_dep)
	g++ $(flags) $(build_dir)/evaluator_test.o $(build_dir)/lexer.o $(build_dir)/parser.o \
	$(build_dir)/token.o $(build_dir)/ast.o $(build_dir)/evaluator.o $(build_dir)/gcollector.o \
	$(build_dir)/object.o $(build_dir)/environment.o -o $(exec_dir)/evaluator_test


builtin_test: build/ bin/ $(builtin_dep)
	g++ $(flags) $(build_dir)/builtin_test.o $(build_dir)/lexer.o $(build_dir)/parser.o \
	$(build_dir)/token.o $(build_dir)/ast.o $(build_dir)/evaluator.o $(build_dir)/gcollector.o \
	$(build_dir)/object.o $(build_dir)/environment.o -o $(exec_dir)/builtin_test




test: lexer_test parser_test evaluator_test builtin_test
	$(exec_dir)/lexer_test
	$(exec_dir)/parser_test
	$(exec_dir)/evaluator_test
	$(exec_dir)/builtin_test

# Utility

clean:
	rm build/*.o bin/*

build/:
	mkdir -p build

bin/:
	mkdir -p bin

	