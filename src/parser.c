#include <parser.h>
#include <stdlib.h>
#include <stdbool.h>
#include <list.h>
#include <stdio.h>
#include <string.h>

void next_token_p(parser_t* p) {
  if (p->curr_token != NULL) {
    push_back(p->used_tokens, p->curr_token);
  }
  p->curr_token = p->peek_token;
  p->peek_token = next_token(p->l);
}

parser_t* new_parser(lexer_t* l) {
  parser_t* p = malloc(sizeof(parser_t));
  if (p == NULL)
    return NULL;
  
  p->l = l;
  p->curr_token = NULL;
  p->peek_token = NULL;
  p->used_tokens = initialize_list();
  p->errors = initialize_list();

  next_token_p(p);
  next_token_p(p);

  return p;
}

bool peek_token_is(parser_t* p, token_type_t t) {
  return p->peek_token->type == t;
}

void peek_error(parser_t* p, token_type_t t) {
  char msg[256];
  snprintf(msg, sizeof(msg), "expected next token to be %d, got %d instead"
          ,t, p->peek_token->type);

  char* str = malloc(257 * sizeof(char));
  strcpy(str, msg);

  push_back(p->errors, str);

}

bool expect_peek(parser_t* p, token_type_t t) {
  if (peek_token_is(p, t)) {
    next_token_p(p); // advance to next token
    return true;
  }
  peek_error(p, t);
  return false;
}


bool curr_token_is(parser_t* p, token_type_t t) {
  return p->curr_token->type == t;
}

var_stmt_t* parse_var_stmt(parser_t* p) {
  var_stmt_t* var_stmt = malloc(sizeof(var_stmt_t));
  if (var_stmt == NULL)
    return NULL;
  
  var_stmt->token = p->curr_token;

  if (!expect_peek(p, IDENT)) {
    return NULL;
  }

  identifier_t* i = malloc(sizeof(identifier_t));
  if (i == NULL)
    return NULL;
  
  i->token = p->curr_token;
  i->value = p->curr_token->literal;
  var_stmt->name = i;

  if (!expect_peek(p, ASSIGN)) {
    return NULL;
  }

  // skipping expressions for now
  while (!curr_token_is(p, SEMICOLON)) {
    next_token_p(p);
  }

  return var_stmt;
}

program_t* parse_program(parser_t* p) {
  program_t* program = malloc(sizeof(program_t));
  if (program == NULL)
    return NULL;
  
  program->statements = initialize_list();
  while (!curr_token_is(p, EOI)) {
    statement_t* stmt = malloc(sizeof(statement_t));
    if (stmt == NULL)
      return NULL;
    
    switch(p->curr_token->type) {
      case VAR:
        stmt->type = VAR_S;
        stmt->stmt.var_stmt = parse_var_stmt(p);
        if (stmt != NULL) {
          push_back(program->statements, stmt);
        }
        break;
      default:
        printf("We hit default\n");
    }
    next_token_p(p);
  }

  
  return program;
}
