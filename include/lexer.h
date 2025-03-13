#ifndef MCSCRIPT_V3_LEXER_H
#define MCSCRIPT_V3_LEXER_H

#include <token.h>

typedef struct {
  char ch; // the current character being examined
  const char* input; // source code
  int position; // points to the current char in input
  int reader_position; // points to the next char in input
} lexer_t;

void read_char(lexer_t* l); // consume the current char
lexer_t* new_lexer(const char* input);
token_t* next_token(lexer_t* l);

#endif //MCSCRIPT_V3_LEXER_H 