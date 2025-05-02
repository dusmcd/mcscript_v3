#ifndef MCSCRIPT_V3_TOKEN_K
#define MCSCRIPT_V3_TOKEN_K

#include <string>
#include <unordered_map>

enum class TokenType : int {
  // operators
  ASSIGN,
  PLUS,
  MINUS,
  EQ,
  NOT_EQ,
  GT, // >
  LT, // <
  BANG, // !
  SLASH,
  ASTERISK,

  // objects
  INT,
  STRING,

  // delimiters
  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,
  LBRACKET,
  RBRACKET,
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
};


class Token {
  public:
    Token(TokenType type, char ch);
    Token(TokenType type, std::string literal);
    inline TokenType GetType() const {
      return type_;
    }

    inline std::string GetLiteral() const {
      return literal_;
    }
    static TokenType LookUpIdent(std::string literal);

  private:
    TokenType type_;
    std::string literal_;
};



static const std::unordered_map<std::string, TokenType> keywords = {
  {"var", TokenType::VAR},
  {"function", TokenType::FUNCTION},
  {"if", TokenType::IF},
  {"else", TokenType::ELSE},
  {"return", TokenType::RETURN},
  {"true", TokenType::TRUE},
  {"false", TokenType::FALSE},
};



#endif //MCSCRIPT_V3_TOKEN_K
