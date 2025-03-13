#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_next_char() {
  const char* input = "(){},;=+";

  typedef struct {
    token_type_t expected_type;
    const char* expected_literal;
  } test_t;

  test_t tests[] = {
    {.expected_type = LPAREN, .expected_literal = "("},
    {.expected_type = RPAREN, .expected_literal = ")"},
    {.expected_type = LBRACE, .expected_literal = "{"},
    {.expected_type = RBRACE, .expected_literal = "}"},
    {.expected_type = COMMA, .expected_literal = ","},
    {.expected_type = SEMICOLON, .expected_literal = ";"},
    {.expected_type = ASSIGN, .expected_literal = "="},
    {.expected_type = PLUS, .expected_literal = "+"},
    {.expected_type = EOI, .expected_literal = ""},
  };

  lexer_t* lex = new_lexer(input);
  for (int i = 0; i < 9; i++) {
    token_t* tok = next_char(lex);
    token_type_t expected_type = tests[i].expected_type;
    const char* expected_literal = tests[i].expected_literal;

    if (tok->type != expected_type) {
      printf("test[%d]: wrong token_type_t. expected: %d, actual: %d\n",
        i, tests[i].expected_type, tok->type);
      return;
    }

    if (strcmp(expected_literal, tok->literal) != 0) {
      printf("test[%d]: wrong literal. expected: %s, actual: %s\n",
        i, tests[i].expected_literal, tok->literal);
      return;
    }


    free(tok);
  }
  free(lex);
  printf("test_next_char passed\n");
}

int main() {
  test_next_char();

  return 0;
}