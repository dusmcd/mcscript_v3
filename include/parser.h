#ifndef MCSCRIPT_V3_PARSER_H
#define MCSCRIPT_V3_PARSER_H

#include <lexer.h>
#include <ast.h>

typedef struct {
  lexer_t* l;
  token_t* curr_token;
  token_t* peek_token;
  list_t* used_tokens;
} parser_t;

parser_t* new_parser(lexer_t* l);
program_t* parse_program(parser_t* p);



#endif //MCSCRIPT_V3_PARSER_H