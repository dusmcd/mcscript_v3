#include <lexer.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
  printf("McScript v3.0 Programming Language\n");
  printf("Enter commands:\n");

  while (true) {
    printf(">> ");
    char input[256];
    fgets(input, sizeof(input), stdin);

    lexer_t* l = new_lexer(input);

    for (token_t* tok = next_token(l); tok->type != EOI; tok = next_token(l)) {
      printf("{type: %d, literal: %s}\n", tok->type, tok->literal);
      free(tok);
      tok = NULL;
    }

    free(l);
    l = NULL;
  }


  return 0;
}