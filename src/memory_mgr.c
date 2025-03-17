#include <memory_mgr.h>
#include <ast.h>
#include <stdlib.h>

void dispose(heap_t garbage) {
  // free the lexer
  if (garbage.l != NULL) {
    free(garbage.l);
    garbage.l = NULL;
  }

  // free all tokens
  if (garbage.p != NULL && garbage.p->used_tokens != NULL) {
    node_t* current = garbage.p->used_tokens->head;
    while (current != NULL) {
      node_t* next = current->next;
      token_t* tok = (token_t*)current->val;
      
      // free the actual token
      free(tok->literal);
      free(tok);

      current->val = NULL;
      free(current); // free the node
      current = next;
    }
    free(garbage.p->used_tokens); // free the list
    garbage.p->used_tokens = NULL;
  }

  // free the list of errors
  if (garbage.p != NULL && garbage.p->errors != NULL) {
    node_t* current = garbage.p->errors->head;
    while (current != NULL) {
      char* msg = (char*)current->val;
      free(msg);
      node_t* next = current->next;
      free(current);
      current = next;
    }

    free(garbage.p->errors);
  }

  // free the parser
  if (garbage.p != NULL) {
    free(garbage.p);
    garbage.p = NULL;
  }

  // free all statements and associated components
  if (garbage.program != NULL && garbage.program->statements != NULL) {
    node_t* current = garbage.program->statements->head;
    while (current != NULL) {
      node_t* next = current->next;
      statement_t* stmt = (statement_t*)current->val;
      switch(stmt->type) {
        case VAR_S:
          free(stmt->stmt.var_stmt->name); // free *identifier_t
          free(stmt->stmt.var_stmt); // free var_stmt_t*
      }
      free(current); // free the node
      current = next;
    }
    free(garbage.program->statements); // free the list
    garbage.program->statements = NULL;
  }

  // free the program
  if (garbage.program != NULL) {
    free(garbage.program);
    garbage.program = NULL;
  }

}