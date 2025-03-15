#include <parser.h>
#include <stdio.h>
#include <stdbool.h>
#include <memory_mgr.h>
#include <stdlib.h>

bool test_var_statement(var_stmt_t* vs) {
  return true;
}

void test_var_statements() {
  const char* input = "var x = 10;"
                      "var num = 5;";
  
  lexer_t* l = new_lexer(input);
  if (l == NULL) {
    printf("bad memory allocation lexer\n");
    return;
  }
      
  parser_t* p = new_parser(l);
  if (p == NULL) {
    printf("bad memory allocaiton parser\n");
    free(l);
    return;
  }

  program_t* program = parse_program(p);
  if (program == NULL) {
    printf("bad memory allocation program\n");
    free(l);
    free(p);
    return;
  }

  if (program->statements->size < 2) {
    printf("program should have 2 statements");
    free(l);
    free(p);
    free(program);
    return;
  }

  typedef struct {
    const char* literal;
  } test_t;

  test_t tests[] = {
    {.literal = "x"},
    {.literal = "num"},
  };

  node_t* current = program->statements->head;
  for (int i = 0; i < 2; i++) {
    statement_t* stmt = (statement_t*)current->val;
    var_stmt_t* vs = stmt->stmt.var_stmt;
    if (!test_var_statement(vs)) {
      return;
    }
    printf("testing var statement %d\n", i + 1);
    current = current->next;
  }

  heap_t garbage = {.l = l, .p = p, .program = program};
  dispose(garbage);
}


int main() {
  test_var_statements();

  return 0;
}