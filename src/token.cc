#include <token.h>


TokenType Token::LookUpIdent(std::string literal) {
  if (keywords.count(literal) > 0)
    return keywords.at(literal);

  return TokenType::IDENT;
}

Token::Token(TokenType type, char ch) {
  type_ = type;
  std::string literal = "";
  literal += ch;
  literal_ = literal;
}

Token::Token(TokenType type, std::string literal) : type_(type), literal_(literal) {
  // empty
}