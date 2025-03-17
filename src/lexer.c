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
  char* str = malloc(2 * sizeof(char));
  str[0] = ch;
  str[1] = '\0';
  tok->literal = str;

  return tok;
}

bool is_letter(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_'; 
}

bool is_digit(char ch) {
  return ch >= '0' && ch <= '9';
}

char* read_ident(lexer_t* l) {
  int i = 0;
  char* str = malloc(100 * sizeof(char));
  if (str == NULL)
    return NULL;
  
  while (is_letter(l->ch)) {
    str[i] = l->ch;
    read_char(l);
    i++;
  }
  str[i] = '\0';

  return str;
}

char* read_number(lexer_t* l) {
  int i = 0;
  char* str = malloc(sizeof(char) * 100);
  if (str == NULL)
    return NULL;

  while (is_digit(l->ch)) {
    str[i] = l->ch;
    read_char(l);
    i++;
  }
  str[i] = '\0';

  return str;
}

void skip_whitespace(lexer_t* l) {
  char ch = l->ch;
  while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {
    read_char(l);
    ch = l->ch;
  }
}


token_t* next_token(lexer_t* l) {
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
        char* str = malloc(3 * sizeof(char));
        char buff[] = {temp, l->ch, '\0'};
        strcpy(str, buff);
        tok->literal = str;
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
        char* str = malloc(3 * sizeof(char));
        char buff[] = {temp, l->ch, '\0'};
        strcpy(str, buff);
        tok->literal = str;
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
      
      const char* end = "";
      char* str = malloc(2 * sizeof(char));
      strcpy(str, end);
      tok->literal = str;
      tok->type = EOI;
      break;
    default:
      if (is_letter(l->ch)) {
        tok = malloc(sizeof(token_t));
        if (tok == NULL)
          return NULL;

        tok->literal = read_ident(l);
        tok->type = look_up_ident(tok->literal);
        return tok;
      } else if (is_digit(l->ch)) {
        tok = malloc(sizeof(token_t));
        if (tok == NULL)
          return NULL;
        
        tok->type = INT;
        tok->literal = read_number(l);
        return tok;
      } else {
        tok = new_token(ILLEGAL, l->ch);
      }
  }
  read_char(l);

  return tok;
  
}