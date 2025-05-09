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

std::string Token::GetTokenString(TokenType type) {
  size_t idx = static_cast<size_t>(type);
  if (idx > tokenStrs.size() - 1) {
    return "";
  }

  return tokenStrs[idx];
}
