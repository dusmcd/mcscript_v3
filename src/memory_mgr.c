#include <memory_mgr.h>
#include <ast.h>
#include <stdlib.h>

void free_tokens(list_t* tokens) {
  node_t* current = tokens->head;
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
      free(tokens); // free the list
      tokens = NULL;

}

void free_errors(list_t* errors) {
  node_t* current = errors->head;
    while (current != NULL) {
      char* msg = (char*)current->val;
      free(msg);
      node_t* next = current->next;
      free(current);
      current = next;
    }

    free(errors);
}

void free_statements(list_t* statements) {
  node_t* current = statements->head;
    while (current != NULL) {
      node_t* next = current->next;
      statement_t* stmt = (statement_t*)current->val;
      switch(stmt->type) {
        case VAR_S:
          free(stmt->stmt.var_stmt->name); // free *identifier_t
          free(stmt->stmt.var_stmt); // free var_stmt_t*
          free(stmt);
          break;
        case RETURN_S:
          free(stmt->stmt.return_stmt);
          free(stmt);
          break;
      }
      free(current); // free the node
      current = next;
    }
    free(statements); // free the list

}

void dispose(heap_t garbage) {
  // free the lexer
  if (garbage.l != NULL) {
    free(garbage.l);
    garbage.l = NULL;
  }

  // free all tokens
  if (garbage.p != NULL && garbage.p->used_tokens != NULL) {
    free_tokens(garbage.p->used_tokens);
  }

  // free the list of errors
  if (garbage.p != NULL && garbage.p->errors != NULL) {
    free_errors(garbage.p->errors);
  }

  // free the parser
  if (garbage.p != NULL) {
    free(garbage.p);
    garbage.p = NULL;
  }

  // free all statements and associated components
  if (garbage.program != NULL && garbage.program->statements != NULL) {
    free_statements(garbage.program->statements);
  }

  // free the program
  if (garbage.program != NULL) {
    free(garbage.program);
    garbage.program = NULL;
  }

}