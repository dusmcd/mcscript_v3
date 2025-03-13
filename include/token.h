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

  // objects
  INT,

  // delimiters
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  COMMA,
  SEMICOLON,

  // special
  EOI,
  ILLEGAL,

  // keywords
  VAR,
  FUNCTION,
  IF,
  ELSE,
  RETURN,
  TRUE,
  FALSE,
  IDENT
} token_type_t;

typedef struct {
  token_type_t type;
  const char* literal;
} token_t;



token_type_t look_up_ident(const char* literal);
static const token_t keywords[] = {
  {.literal = "var", .type = VAR},
  {.literal = "function", .type = FUNCTION},
  {.literal = "if", .type = IF},
  {.literal = "else", .type = ELSE},
  {.literal = "return", .type = RETURN},
  {.literal = "true", .type = TRUE},
  {.literal = "false", .type = FALSE},
};


#endif //MCSCRIPT_V3_TOKEN_K