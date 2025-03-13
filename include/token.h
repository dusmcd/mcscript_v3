#ifndef MCSCRIPT_V3_TOKEN_K
#define MCSCRIPT_V3_TOKEN_K

typedef enum {
  // operators
  ASSIGN,
  PLUS,
  MINUS,
  EQ,
  NOT_EQ,
  GT, // >
  LT, // <

  // delimiters
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  COMMA,
  SEMICOLON,

  // special
  EOI,

  // keywords
  VAR,
  FUNCTION,
  IF,
  ELSE,
  RETURN,
  TRUE,
  FALSE
} token_type_t;

typedef struct {
  token_type_t type;
  const char* literal;
} token_t;

#endif //MCSCRIPT_V3_TOKEN_K