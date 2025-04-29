#include <lexer.h>
#include <memory>

Lexer::Lexer(std::string input) {
    
  input_ = input;
  position_ = -1;
  reader_position_ = 0;
  ReadChar_();
}

void Lexer::ReadChar_() {
  int size = input_.size();
  if (position_ >= size) {
    ch_ = 0;
  }
  else {
    ch_ = input_[reader_position_];
    position_ = reader_position_;
    reader_position_++;
  }
}

std::shared_ptr<Token> Lexer::NewToken_(TokenType type, char ch) {
  auto tok = std::make_shared<Token>(type, ch);
  if (tok == nullptr) {
    return nullptr;
  }

  return tok;
}

bool Lexer::IsLetter_(char ch) {
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_'; 
}

bool Lexer::IsDigit_(char ch) {
  return ch >= '0' && ch <= '9';
}

std::string Lexer::ReadIdent_() {
  std::string str = "";
  
  while (IsLetter_(ch_)) {
    str += ch_;
    ReadChar_();
  }

  return str;
}

std::string Lexer::ReadNumber_() {
  std::string str = "";

  while (IsDigit_(ch_)) {
    str += ch_;
    ReadChar_();
  }

  return str;
}

void Lexer::SkipWhiteSpace_() {
  char ch = ch_;
  while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {
    ReadChar_();
    ch = ch_;
  }
}


std::shared_ptr<Token> Lexer::NextToken()  {
  std::shared_ptr<Token> tok;
    
  SkipWhiteSpace_();

  switch(ch_) {
    case '=':
      if (input_[reader_position_] == '=') {
        char temp = ch_;
        ReadChar_();

        char buff[] = {temp, ch_, '\0'};
        std::string str(buff);
        tok = std::make_shared<Token>(TokenType::EQ, str);
      } else {
        tok = NewToken_(TokenType::ASSIGN, ch_);
      }
      break;
    case '!':
      if (input_[reader_position_] == '=') {
        char temp = ch_;
        ReadChar_();
        char buff[] = {temp, ch_, '\0'};
              
        std::string str(buff);
        tok = std::make_shared<Token>(TokenType::NOT_EQ, str);
      } else {
        tok = NewToken_(TokenType::BANG, ch_);
      }
      break;
    case '"':
      tok = std::make_shared<Token>(TokenType::STRING, ReadString_());
      break;
    case '<':
      tok = NewToken_(TokenType::LT, ch_);
      break;
    case '>':
      tok = NewToken_(TokenType::GT, ch_);
      break;
    case '+':
      tok = NewToken_(TokenType::PLUS, ch_);
      break;
    case '-':
      tok = NewToken_(TokenType::MINUS, ch_);
      break;
    case '*':
      tok = NewToken_(TokenType::ASTERISK, ch_);
      break;
    case '/':
      tok = NewToken_(TokenType::SLASH, ch_);
      break;
    case '(':
      tok = NewToken_(TokenType::LPAREN, ch_);
      break;
    case ')':
      tok = NewToken_(TokenType::RPAREN, ch_);
      break;
    case '{':
      tok = NewToken_(TokenType::LBRACE, ch_);
      break;
    case '}':
      tok = NewToken_(TokenType::RBRACE, ch_);
      break;
    case ',':
      tok = NewToken_(TokenType::COMMA, ch_);
      break;
    case ';':
      tok = NewToken_(TokenType::SEMICOLON, ch_);
      break;
    case '\0':
      tok = std::make_shared<Token>(TokenType::EOI, "");
      break;
    default:
      if (IsLetter_(ch_)) {
        std::string literal = ReadIdent_();
        TokenType type = Token::LookUpIdent(literal);
        tok = std::make_shared<Token>(type, literal);
        return tok;
      } else if (IsDigit_(ch_)) {
        std::string literal = ReadNumber_();
        tok = std::make_shared<Token>(TokenType::INT, literal);
        return tok;
      } else {
        tok = NewToken_(TokenType::ILLEGAL, ch_);
      }
  }
  ReadChar_();

  return tok;
  
}


std::string Lexer::ReadString_() {
  std::string result = "";
  ReadChar_(); // consume the opening quote symbol
  while (true) {
    if (ch_ == '"' || ch_ == '\0') {
      break;
    } 
    result += ch_;
    ReadChar_();
  }

  return result;
}