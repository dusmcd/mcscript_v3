#ifndef MCSCRIPT_V3_PARSER_H
#define MCSCRIPT_V3_PARSER_H

#include <lexer.h>
#include <ast.h>
#include <memory>

class Parser {
  
  public:
    Parser(std::shared_ptr<Lexer> l);
    std::unique_ptr<Program> ParseProgram();

    inline std::vector<std::string> GetErrors() {
      return errors_;
    }

  private:
    void NextToken_(); 
    bool PeekTokenIs_(TokenType t);
    void PeekError_(TokenType t);
    bool ExpectPeek_(TokenType t);
    bool CurrTokenIs_(TokenType t);
    std::shared_ptr<Statement> ParseStatement_();
    std::shared_ptr<VarStatement> ParseVarStatement_();
    std::shared_ptr<ReturnStatement> ParseReturnStatement_(); 
    std::shared_ptr<Token> curr_token_;
    std::shared_ptr<Token> peek_token_;
    std::vector<std::string> errors_;
    std::shared_ptr<Lexer> l_;
};


#endif //MCSCRIPT_V3_PARSER_H