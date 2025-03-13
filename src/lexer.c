#include <lexer.h>
#include <stdlib.h>
#include <string.h>

lexer_t* new_lexer(const char* input) {
  lexer_t* l = malloc(sizeof(lexer_t));
  if (l == NULL)
    return NULL;
  
  l->input = input;
  l->position = -1;
  l->reader_position = 0;
  read_char(l);

  return l;
}

void read_char(lexer_t* l) {
  int size = strlen(l->input);
  if (l->position >= size) {
    l->ch = 0;
  }
  else {
    l->ch = l->input[l->reader_position];
    l->position = l->reader_position;
    l->reader_position++;
  }
}

token_t* new_token(token_type_t type, char ch) {
  token_t* tok = malloc(sizeof(token_t));
  if (tok == NULL)
    return NULL;
  
  tok->type = type;
  const char buff[2] = {ch, '\0'};
  tok->literal = buff;

  return tok;
}


token_t* next_char(lexer_t* l) {
  token_t* tok;
    
  switch(l->ch) {
    case '=':
      tok = new_token(ASSIGN, l->ch);
      break;
    case '+':
      tok = new_token(PLUS, l->ch);
      break;
    case '(':
      tok = new_token(LPAREN, l->ch);
      break;
    case ')':
      tok = new_token(RPAREN, l->ch);
      break;
    case '{':
      tok = new_token(LBRACE, l->ch);
      break;
    case '}':
      tok = new_token(RBRACE, l->ch);
      break;
    case ',':
      tok = new_token(COMMA, l->ch);
      break;
    case ';':
      tok = new_token(SEMICOLON, l->ch);
      break;
    case '\0':
      tok = malloc(sizeof(token_t));
      if (tok == NULL)
        return NULL;
      
      tok->literal = "";
      tok->type = EOI;
      break;
  }
  read_char(l);

  return tok;
  
}