#include <lexer.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

bool is_letter(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_'; 
}

bool is_digit(char ch) {
  return ch >= '0' && ch <= '9';
}

void read_ident(lexer_t* l, char* buff) {
  int i = 0;
  while (is_letter(l->ch)) {
    buff[i] = l->ch;
    read_char(l);
    i++;
  }
  buff[i] = '\0';
}

void read_number(lexer_t* l, char* buff) {
  int i = 0;
  while (is_digit(l->ch)) {
    buff[i] = l->ch;
    read_char(l);
    i++;
  }
  buff[i] = '\0';
}

void skip_whitespace(lexer_t* l) {
  char ch = l->ch;
  while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {
    read_char(l);
    ch = l->ch;
  }
}


token_t* next_char(lexer_t* l) {
  token_t* tok;
    
  skip_whitespace(l);

  switch(l->ch) {
    case '=':
      if (l->input[l->reader_position] == '=') {
        char temp = l->ch;
        read_char(l);
        tok = malloc(sizeof(token_t));
        if (tok == NULL)
          return NULL;
        
        tok->type = EQ;
        const char buff[] = {temp, l->ch, '\0'};
        tok->literal = buff;
      } else {
        tok = new_token(ASSIGN, l->ch);
      }
      break;
    case '!':
      if (l->input[l->reader_position] == '=') {
        char temp = l->ch;
        read_char(l);
        tok = malloc(sizeof(token_t));
        if (tok == NULL)
          return NULL;
        
        tok->type = NOT_EQ;
        const char buff[] = {temp, l->ch, '\0'};
        tok->literal = buff;
      } else {
        tok = new_token(BANG, l->ch);
      }
      break;
    case '<':
      tok = new_token(LT, l->ch);
      break;
    case '>':
      tok = new_token(GT, l->ch);
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
    default:
      if (is_letter(l->ch)) {
        tok = malloc(sizeof(token_t));
        if (tok == NULL)
          return NULL;

        char buff[100];
        read_ident(l, buff);
        tok->literal = buff;
        tok->type = look_up_ident(tok->literal);
        return tok;
      } else if (is_digit(l->ch)) {
        tok = malloc(sizeof(token_t));
        if (tok == NULL)
          return NULL;
        
        tok->type = INT;
        char buff[100];
        read_number(l, buff);
        tok->literal = buff;
        return tok;
      } else {
        tok = new_token(ILLEGAL, l->ch);
      }
  }
  read_char(l);

  return tok;
  
}