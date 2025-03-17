#ifndef MCSCRIPT_V3_AST_H
#define MCSCRIPT_V3_AST_H

#include <token.h>
#include <list.h>

typedef union {
  int val;
} expression_t;

typedef struct {
  token_t* token;
  const char* value;
} identifier_t;

typedef struct {
  token_t* token;
  expression_t value;
  identifier_t* name;
} var_stmt_t;

typedef struct {
  token_t* token;
  expression_t return_value;
} return_stmt_t;

typedef enum {
  VAR_S,
  RETURN_S
} stmt_type_t;

typedef union {
  var_stmt_t* var_stmt;
  return_stmt_t* return_stmt;
} stmt_data_t;

typedef struct {
  stmt_type_t type;
  stmt_data_t stmt;
} statement_t;

typedef struct {
  list_t* statements; // list of statement_t structs
} program_t;


#endif // MCSCRIPT_V3_AST_H