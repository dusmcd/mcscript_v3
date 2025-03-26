#include <parser.h>

/*
===========================================
PUBLIC METHODS
===========================================
*/

/*
  constuctor
*/
Parser::Parser(std::shared_ptr<Lexer> l) {
  l_ = l;
  curr_token_ = nullptr;
  peek_token_ = nullptr;

  RegisterPrefixFns_(GetParseIdentifierFn_(), TokenType::IDENT);
  RegisterPrefixFns_(GetIntegerLiteralFn_(), TokenType::INT);
  RegisterPrefixFns_(GetPrefixExpressionFn_(), TokenType::BANG);
  RegisterPrefixFns_(GetPrefixExpressionFn_(), TokenType::MINUS);
  RegisterPrefixFns_(GetParseGroupedExprFn_(), TokenType::LPAREN);
  RegisterPrefixFns_(GetParseBooleanFn_(), TokenType::TRUE),
  RegisterPrefixFns_(GetParseBooleanFn_(), TokenType::FALSE),

  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::PLUS);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::MINUS);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::SLASH);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::ASTERISK);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::LT);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::GT);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::EQ);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::NOT_EQ);


  NextToken_();
  NextToken_();
}


std::unique_ptr<Program> Parser::ParseProgram(bool skipexpr){
  auto program = std::make_unique<Program>();
  
  while (!CurrTokenIs_(TokenType::EOI)) {
    std::shared_ptr<Statement> stmt = ParseStatement_(skipexpr);
    if (stmt != nullptr) {
      program->AppendStatements(stmt);
    }
    NextToken_();
  }
  
  return program;
}


/*
===============================================================
PRIVATE METHODS
===============================================================
*/

/*
  utility methods
*/
void Parser::NextToken_() {
  curr_token_ = peek_token_;
  peek_token_ = l_->NextToken();
}

bool Parser::PeekTokenIs_(TokenType t) {
  return peek_token_->GetType() == t;
}

void Parser::PeekError_(TokenType t) {
  char msg[256];
  snprintf(msg, sizeof(msg), "expected next token to be %d, got %d instead", 
            static_cast<int>(t), static_cast<int>(peek_token_->GetType()));

  std::string str(msg);
  errors_.push_back(msg);
}

bool Parser::ExpectPeek_(TokenType t) {
  if (PeekTokenIs_(t)) {
    NextToken_();
    return true;
  }
  PeekError_(t);
  return false;
}

bool Parser::CurrTokenIs_(TokenType t) {
  return curr_token_->GetType() == t;
}

Precedence Parser::CurrPrecedence_() {
  if (prMap.count(curr_token_->GetType()) > 0) {
    return prMap.at(curr_token_->GetType());
  }

  return Precedence::LOWEST;
}

Precedence Parser::PeekPrecedence_() {
  if (prMap.count(peek_token_->GetType()) > 0) {
    return prMap.at(peek_token_->GetType());
  }

  return Precedence::LOWEST;
}

/*
  statement parsing
*/
std::shared_ptr<VarStatement> Parser::ParseVarStatement_(bool skipexpr) {
  auto vs = std::make_shared<VarStatement>(curr_token_);

  if (!ExpectPeek_(TokenType::IDENT)) {
    return nullptr;
  }

  auto i = std::make_shared<Identifier>(curr_token_->GetLiteral(), curr_token_);
  
  vs->SetName(i);

  if (!ExpectPeek_(TokenType::ASSIGN)) { 
    return nullptr;
  }

  if (skipexpr) {
    while (!CurrTokenIs_(TokenType::SEMICOLON)) {
      NextToken_();
    }
  }

  return vs;
}

std::shared_ptr<ReturnStatement> Parser::ParseReturnStatement_(bool skipexpr) {
  auto rs = std::make_shared<ReturnStatement>(curr_token_);

  if (skipexpr) {
    while (!CurrTokenIs_(TokenType::SEMICOLON)) {
      NextToken_();
    }
  }

  return rs;
}

std::shared_ptr<Statement> Parser::ParseStatement_(bool skipexpr) {
  std::shared_ptr<Statement> stmt;
  switch(curr_token_->GetType()) {
      case TokenType::VAR:
        stmt = ParseVarStatement_(skipexpr);
        break;
      case TokenType::RETURN:
        stmt = ParseReturnStatement_(skipexpr);
        break;
      default:
        stmt = ParseExpressionStatement_();
    }

  return stmt;
}

std::shared_ptr<ExpressionStatement> Parser::ParseExpressionStatement_() {
  auto stmt = std::make_shared<ExpressionStatement>(curr_token_);

  stmt->SetExpression(ParseExpression_(Precedence::LOWEST));

  if (PeekTokenIs_(TokenType::SEMICOLON)) {
    NextToken_();
  }

  return stmt;
}

/*
  expression parsing
*/

std::shared_ptr<BooleanExpression> Parser::ParseBooleanExpression_() {
  return std::make_shared<BooleanExpression>(curr_token_, CurrTokenIs_(TokenType::TRUE));
}

prefixParseFn Parser::GetParseBooleanFn_() {
  prefixParseFn fn = std::bind(&Parser::ParseBooleanExpression_, this);
  return fn;
}


std::shared_ptr<Identifier> Parser::ParseIdentifier_() {
  auto i = std::make_shared<Identifier>(curr_token_->GetLiteral(), curr_token_);
  return i;
}

prefixParseFn Parser::GetParseIdentifierFn_() {
  prefixParseFn fn = std::bind(&Parser::ParseIdentifier_, this);
  return fn;
}

std::shared_ptr<Expression> Parser::ParseGroupedExpression_() {
  NextToken_();
  std::shared_ptr<Expression> exp = ParseExpression_(Precedence::LOWEST);

  if (!ExpectPeek_(TokenType::RPAREN)) {
    return nullptr;
  }

  return exp;
}

prefixParseFn Parser::GetParseGroupedExprFn_() {
  prefixParseFn fn = std::bind(&Parser::ParseGroupedExpression_, this);
  return fn;
}

std::shared_ptr<Expression> Parser::ParseExpression_(Precedence pr) {
  if (prefixParseFns_.count(curr_token_->GetType()) < 1) {
    char buff[256];
    snprintf(buff, sizeof(buff), "no parser function for type %d",
           static_cast<int>(curr_token_->GetType()));
    std::string msg(buff);
    errors_.push_back(msg);
    return nullptr;
  }

  prefixParseFn prefix = prefixParseFns_.at(curr_token_->GetType());
  if (prefix == nullptr) {
    return nullptr;
  }
  std::shared_ptr<Expression> leftExp = prefix();

  while (!PeekTokenIs_(TokenType::SEMICOLON) && pr < PeekPrecedence_()) {
    infixParseFn infix = infixParseFns_.count(peek_token_->GetType()) > 0 ? 
                        infixParseFns_.at(peek_token_->GetType()) : nullptr;
    if (infix == nullptr)
      return nullptr;

    NextToken_();
    leftExp = infix(leftExp);
  }

  return leftExp;
}



std::shared_ptr<IntegerLiteral> Parser::ParseIntegerLiteral_() {
  auto il = std::make_shared<IntegerLiteral>(curr_token_);

  long val = atol(curr_token_->GetLiteral().c_str());
  if (val == 0 && curr_token_->GetLiteral().compare("0") != 0) {
    std::string msg = "could not parse integer literal";
    errors_.push_back(msg);
    return nullptr;
  }
  il->SetValue(val);

  return il;
}

prefixParseFn Parser::GetIntegerLiteralFn_() {
  prefixParseFn fn = std::bind(&Parser::ParseIntegerLiteral_, this);
  return fn;
}

std::shared_ptr<PrefixExpression> Parser::ParsePrefixExpression_() {
  auto pe = std::make_shared<PrefixExpression>(curr_token_, curr_token_->GetLiteral());

  NextToken_();
  pe->SetRight(ParseExpression_(Precedence::PREFIX));

  return pe;
}

prefixParseFn Parser::GetPrefixExpressionFn_() {
  prefixParseFn fn = std::bind(&Parser::ParsePrefixExpression_, this);
  return fn;
}

std::shared_ptr<InfixExpression> Parser::ParseInfixExpression_
            (std::shared_ptr<Expression> left) {
  auto infix = std::make_shared<InfixExpression>(curr_token_, curr_token_->GetLiteral(), left);

  Precedence pr = CurrPrecedence_();
  NextToken_();
  infix->SetRight(ParseExpression_(pr));
  return infix;
}

infixParseFn Parser::GetInfixExpressionFn_() {
  infixParseFn fn = std::bind(&Parser::ParseInfixExpression_, this, std::placeholders::_1);
  return fn;
}

