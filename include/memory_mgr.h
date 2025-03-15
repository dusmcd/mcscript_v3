#ifndef MCSCRIPT_V3_MEMORY_MGR_H
#define MCSCRIPT_V3_MEMORY_MGR_H

#include <lexer.h>
#include <parser.h>


typedef struct {
  lexer_t* l;
  parser_t* p;
  program_t* program;
} heap_t;

void dispose(heap_t garbage);



#endif // MCSCRIPT_V3_MEMORY_MGR_H