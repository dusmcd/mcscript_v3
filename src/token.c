#include <token.h>
#include <string.h>


token_type_t look_up_ident(const char* literal) {
  for (int i = 0; i < 7; i++) {
    if (strcmp(literal, keywords[i].literal) == 0)
      return keywords[i].type;
  }
  return IDENT;
}