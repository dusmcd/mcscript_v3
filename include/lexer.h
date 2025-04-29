#ifndef MCSCRIPT_V3_LEXER_H
#define MCSCRIPT_V3_LEXER_H

#include <token.h>
#include <string>
#include <memory>

class Lexer {
  public:
    Lexer(std::string input);
    std::shared_ptr<Token> NextToken();

  private:
    void ReadChar_(); // consume the current char
    std::string ReadString_();
    bool IsLetter_(char ch);
    bool IsDigit_(char ch);
    std::shared_ptr<Token> NewToken_(TokenType type, char ch);
    std::string ReadIdent_();
    std::string ReadNumber_();
    void SkipWhiteSpace_();
    char ch_; // the current character being examined
    std::string input_; // source code
    int position_; // points to the current char in input
    int reader_position_; // points to the next char in input
    
};



#endif //MCSCRIPT_V3_LEXER_H 