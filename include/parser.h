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
  ASSIGN,
  PREFIX,
  CALL,
  INDEX
};


class Parser {
  
  public:
    Parser(std::shared_ptr<Lexer> l);
    std::shared_ptr<Program> ParseProgram();

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

    std::shared_ptr<Statement> ParseStatement_();
    std::shared_ptr<VarStatement> ParseVarStatement_();
    std::shared_ptr<ReturnStatement> ParseReturnStatement_(); 
    std::shared_ptr<ForStatement> ParseForStatement_();
    std::shared_ptr<ExpressionStatement> ParseExpressionStatement_();
    std::shared_ptr<Expression> ParseExpression_(Precedence pr);
    std::shared_ptr<Identifier> ParseIdentifier_();
    std::shared_ptr<IntegerLiteral> ParseIntegerLiteral_();
    std::shared_ptr<PrefixExpression> ParsePrefixExpression_();
    std::shared_ptr<InfixExpression> ParseInfixExpression_(std::shared_ptr<Expression> left);
    std::shared_ptr<Expression> ParseGroupedExpression_();
    std::shared_ptr<BooleanExpression> ParseBooleanExpression_();
    std::shared_ptr<IfExpression> ParseIfExpression_();
    std::shared_ptr<BlockStatement> ParseBlockStatement_();
    std::shared_ptr<FunctionLiteral> ParseFunctionLiteral_();
    std::shared_ptr<CallExpression> ParseCallExpression_(std::shared_ptr<Expression> func);
    std::vector<std::shared_ptr<Expression>> ParseCallParameters_();
    std::shared_ptr<StringLiteral> ParseStringLiteral_();
    std::shared_ptr<ArrayLiteral> ParseArrayLiteral_();
    std::vector<std::shared_ptr<Expression>> ParseExpressionList_(TokenType type);
    std::shared_ptr<IndexExpression> ParseIndexExpression_(std::shared_ptr<Expression> idx);
    std::shared_ptr<AssignExpression> ParseAssignExpression_(std::shared_ptr<Expression> left);
    infixParseFn GetParseAssignExpressionFn_();
    infixParseFn GetParseIndexExpression_();
    prefixParseFn GetParseArrayLiteralFn_();
    prefixParseFn GetParseStringLiteralFn_();
    infixParseFn GetParseCallExpressionFn_();
    prefixParseFn GetParseFunctionLiteralFn_();
    prefixParseFn GetParseIfExpression_();
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
  {TokenType::LPAREN, Precedence::CALL},
  {TokenType::LBRACKET, Precedence::INDEX},
  {TokenType::ASSIGN, Precedence::ASSIGN}
};


#endif //MCSCRIPT_V3_PARSER_H
