#ifndef MCSCRIPT_V3_PARSER_H
#define MCSCRIPT_V3_PARSER_H

#include <lexer.h>
#include <ast.h>
#include <memory>
#include <functional>
#include <unordered_map>

using prefixParseFn = std::function<std::shared_ptr<Expression> ()>;
using infixParseFn = std::function<std::shared_ptr<Expression>(std::shared_ptr<Expression>)>;

enum class Precedence : int {
  LOWEST,
  EQUALS,
  LESSGREATER,
  SUM,
  PRODUCT,
  PREFIX,
  CALL 
};


class Parser {
  
  public:
    Parser(std::shared_ptr<Lexer> l);
    std::unique_ptr<Program> ParseProgram(bool skipexpr = false);

    inline std::vector<std::string> GetErrors() {
      return errors_;
    }

  private:
    void NextToken_(); 
    bool PeekTokenIs_(TokenType t);
    void PeekError_(TokenType t);
    bool ExpectPeek_(TokenType t);
    bool CurrTokenIs_(TokenType t);

    inline void RegisterPrefixFns_(prefixParseFn fn, TokenType t) {
      prefixParseFns_[t] = fn;
    }

    inline void RegisterInfixFns_(infixParseFn fn, TokenType t) {
      infixParseFns_[t] = fn;
    }

    std::shared_ptr<Statement> ParseStatement_(bool);
    std::shared_ptr<VarStatement> ParseVarStatement_(bool);
    std::shared_ptr<ReturnStatement> ParseReturnStatement_(bool); 
    std::shared_ptr<ExpressionStatement> ParseExpressionStatement_();
    std::shared_ptr<Expression> ParseExpression_(Precedence pr);
    std::shared_ptr<Identifier> ParseIdentifier_();
    std::shared_ptr<IntegerLiteral> ParseIntegerLiteral_();
    std::shared_ptr<PrefixExpression> ParsePrefixExpression_();
    std::shared_ptr<InfixExpression> ParseInfixExpression_(std::shared_ptr<Expression> left);
    std::shared_ptr<Expression> ParseGroupedExpression_();
    std::shared_ptr<BooleanExpression> ParseBooleanExpression_();
    prefixParseFn GetParseBooleanFn_();
    prefixParseFn GetParseGroupedExprFn_();
    infixParseFn GetInfixExpressionFn_();
    prefixParseFn GetPrefixExpressionFn_();
    prefixParseFn GetIntegerLiteralFn_();
    prefixParseFn GetParseIdentifierFn_();
    Precedence CurrPrecedence_();
    Precedence PeekPrecedence_();
    std::unordered_map<TokenType, prefixParseFn> prefixParseFns_;
    std::unordered_map<TokenType, infixParseFn> infixParseFns_;
    std::shared_ptr<Token> curr_token_;
    std::shared_ptr<Token> peek_token_;
    std::vector<std::string> errors_;
    std::shared_ptr<Lexer> l_;
};

static const std::unordered_map<TokenType, Precedence> prMap = {
  {TokenType::PLUS, Precedence::SUM},
  {TokenType::MINUS, Precedence::SUM},
  {TokenType::ASTERISK, Precedence::PRODUCT},
  {TokenType::SLASH, Precedence::PRODUCT},
  {TokenType::EQ, Precedence::EQUALS},
  {TokenType::NOT_EQ, Precedence::EQUALS},
  {TokenType::LT, Precedence::LESSGREATER},
  {TokenType::GT, Precedence::LESSGREATER},
};


#endif //MCSCRIPT_V3_PARSER_H