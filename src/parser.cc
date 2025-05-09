#include <memory>
#include <parser.h>

/*
===========================================
PUBLIC METHODS
===========================================
*/

/*
  constructor
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
  RegisterPrefixFns_(GetParseIfExpression_(), TokenType::IF);
  RegisterPrefixFns_(GetParseFunctionLiteralFn_(), TokenType::FUNCTION);
  RegisterPrefixFns_(GetParseStringLiteralFn_(), TokenType::STRING);
  RegisterPrefixFns_(GetParseArrayLiteralFn_(), TokenType::LBRACKET);

  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::PLUS);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::MINUS);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::SLASH);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::ASTERISK);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::LT);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::GT);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::EQ);
  RegisterInfixFns_(GetInfixExpressionFn_(), TokenType::NOT_EQ);
  RegisterInfixFns_(GetParseCallExpressionFn_(), TokenType::LPAREN);
  RegisterInfixFns_(GetParseIndexExpression_(), TokenType::LBRACKET);
  RegisterInfixFns_(GetParseAssignExpressionFn_(), TokenType::ASSIGN);



  NextToken_();
  NextToken_();
}


std::shared_ptr<Program> Parser::ParseProgram(){
  auto program = std::make_shared<Program>();
  
  while (!CurrTokenIs_(TokenType::EOI)) {
    std::shared_ptr<Statement> stmt = ParseStatement_();
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

std::shared_ptr<BlockStatement> Parser::ParseBlockStatement_() {
  auto bs = std::make_shared<BlockStatement>(curr_token_);
  NextToken_(); // jumping over left brace

  while (!CurrTokenIs_(TokenType::RBRACE) && !CurrTokenIs_(TokenType::EOI)) {
    std::shared_ptr<Statement> stmt = ParseStatement_();
    if (stmt != nullptr) {
      bs->AppendStatements(stmt);
    }
    NextToken_();
  }

  return bs;
}

std::shared_ptr<VarStatement> Parser::ParseVarStatement_() {
  auto vs = std::make_shared<VarStatement>(curr_token_);

  if (!ExpectPeek_(TokenType::IDENT)) {
    return nullptr;
  }

  auto i = std::make_shared<Identifier>(curr_token_->GetLiteral(), curr_token_);
  
  vs->SetName(i);

  if (!ExpectPeek_(TokenType::ASSIGN)) { 
    return nullptr;
  }

  NextToken_();
  vs->SetValue(ParseExpression_(Precedence::LOWEST));

  if (PeekTokenIs_(TokenType::SEMICOLON)) {
    NextToken_();
  }

  return vs;
}

std::shared_ptr<ReturnStatement> Parser::ParseReturnStatement_() {
  auto rs = std::make_shared<ReturnStatement>(curr_token_);

  NextToken_();
  rs->SetReturnVal(ParseExpression_(Precedence::LOWEST));

  if (PeekTokenIs_(TokenType::SEMICOLON)) {
    NextToken_();
  }
  
  return rs;
}

std::shared_ptr<Statement> Parser::ParseStatement_() {
  std::shared_ptr<Statement> stmt;
  switch(curr_token_->GetType()) {
      case TokenType::VAR:
        stmt = ParseVarStatement_();
        break;
      case TokenType::RETURN:
        stmt = ParseReturnStatement_();
        break;
      case TokenType::FOR:
        stmt = ParseForStatement_();
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

std::shared_ptr<ForStatement> Parser::ParseForStatement_() {
  if (!ExpectPeek_(TokenType::LPAREN)) {
    return nullptr;
  }

  if (!ExpectPeek_(TokenType::VAR)) {
    return nullptr;
  }

  std::shared_ptr<VarStatement> varStmt = ParseVarStatement_();

  if (!CurrTokenIs_(TokenType::SEMICOLON)) {
    char buff[256];
    snprintf(buff, sizeof(buff), "expected token to be %d, but got %d instead",
        static_cast<int>(TokenType::SEMICOLON), static_cast<int>(curr_token_->GetType()));
    errors_.push_back(std::string(buff));
    return nullptr;
  }

  NextToken_();

  std::shared_ptr<Expression> condition = ParseExpression_(Precedence::LOWEST);

  if (!ExpectPeek_(TokenType::SEMICOLON)) {
    return nullptr;
  }

  NextToken_();

  std::shared_ptr<Expression> afterAction = ParseExpression_(Precedence::LOWEST);
  if (!ExpectPeek_(TokenType::RPAREN)) {
    return nullptr;
  }

  if (!ExpectPeek_(TokenType::LBRACE)) {
    return nullptr;
  }

  std::shared_ptr<BlockStatement> block = ParseBlockStatement_();
  auto forStmt = std::make_shared<ForStatement>(curr_token_, varStmt, condition, afterAction, block);

  return forStmt;
}

/*
  expression parsing
*/

std::shared_ptr<AssignExpression> Parser::ParseAssignExpression_(std::shared_ptr<Expression> left) {
  auto exp = std::make_shared<AssignExpression>(left);
  NextToken_(); // jump over '='

  std::shared_ptr<Expression> newVal = ParseExpression_(Precedence::LOWEST);

  exp->SetNewVal(newVal);
  return exp;
}

infixParseFn Parser::GetParseAssignExpressionFn_() {
  infixParseFn fn = std::bind(&Parser::ParseAssignExpression_, this, std::placeholders::_1);
  return fn;
}


std::shared_ptr<IndexExpression> Parser::ParseIndexExpression_(std::shared_ptr<Expression> left) {
  auto exp = std::make_shared<IndexExpression>(left);
  NextToken_(); // jump over left bracket

  std::shared_ptr<Expression> idx = ParseExpression_(Precedence::LOWEST);

  exp->SetIdx(idx);

  if (!ExpectPeek_(TokenType::RBRACKET)) {
    return nullptr;
  }

  return exp;
}

infixParseFn Parser::GetParseIndexExpression_() {
  infixParseFn fn = std::bind(&Parser::ParseIndexExpression_, this, std::placeholders::_1);
  return fn;
}

std::shared_ptr<ArrayLiteral> Parser::ParseArrayLiteral_() {
  auto arr = std::make_shared<ArrayLiteral>(curr_token_);
  arr->SetExps(ParseExpressionList_(TokenType::RBRACKET));
  return arr;
}

prefixParseFn Parser::GetParseArrayLiteralFn_() {
  prefixParseFn fn = std::bind(&Parser::ParseArrayLiteral_, this);
  return fn;
}


std::vector<std::shared_ptr<Expression>> Parser::ParseExpressionList_(TokenType end) {
  std::vector<std::shared_ptr<Expression>> args = {};

  if (PeekTokenIs_(end)) {
    NextToken_();
    return args;
  }

  NextToken_();
  args.push_back(ParseExpression_(Precedence::LOWEST));
  while (PeekTokenIs_(TokenType::COMMA)) {
    NextToken_();
    NextToken_();
    args.push_back(ParseExpression_(Precedence::LOWEST));
  }

  if (!ExpectPeek_(end)) {
    return std::vector<std::shared_ptr<Expression>>{};
  }

  return args;

}


std::shared_ptr<CallExpression> Parser::ParseCallExpression_(std::shared_ptr<Expression> func) {
  auto exp = std::make_shared<CallExpression>(curr_token_, func);
  exp->SetArgs(ParseCallParameters_());
  return exp;
}

infixParseFn Parser::GetParseCallExpressionFn_() {
  infixParseFn fn = std::bind(&Parser::ParseCallExpression_, this, std::placeholders::_1);
  return fn;
}

std::vector<std::shared_ptr<Expression>> Parser::ParseCallParameters_() {
  std::vector<std::shared_ptr<Expression>> args = ParseExpressionList_(TokenType::RPAREN);
  return args;
}


std::shared_ptr<FunctionLiteral> Parser::ParseFunctionLiteral_() {
  auto function = std::make_shared<FunctionLiteral>(curr_token_);

  if (!ExpectPeek_(TokenType::LPAREN)) {
    return nullptr;
  }

  std::vector<std::shared_ptr<Identifier>> params = {};

  if (PeekTokenIs_(TokenType::RPAREN)) {
    // no parameters
    function->SetParameters(params);
  } else {
    NextToken_();
    auto i = std::make_shared<Identifier>(curr_token_->GetLiteral(), curr_token_);
    params.push_back(i);
    while (PeekTokenIs_(TokenType::COMMA)) {
      NextToken_();
      NextToken_();

      auto i = std::make_shared<Identifier>(curr_token_->GetLiteral(), curr_token_);
      params.push_back(i);
    }

    function->SetParameters(params);
  }

  if (!ExpectPeek_(TokenType::RPAREN)) {
    return nullptr;
  }

  if (!ExpectPeek_(TokenType::LBRACE)) {
    return nullptr;
  }

  function->SetBody(ParseBlockStatement_());

  return function;
}

prefixParseFn Parser::GetParseFunctionLiteralFn_() {
  prefixParseFn fn = std::bind(&Parser::ParseFunctionLiteral_, this);
  return fn;
}


std::shared_ptr<IfExpression> Parser::ParseIfExpression_() {
  auto ifExp = std::make_shared<IfExpression>(curr_token_);

  if (!ExpectPeek_(TokenType::LPAREN)) {
    return nullptr;
  }

  NextToken_(); // skip over left paren to condition

  ifExp->SetCondition(ParseExpression_(Precedence::LOWEST));

  if (!ExpectPeek_(TokenType::RPAREN)) {
    return nullptr;
  }

  if (!ExpectPeek_(TokenType::LBRACE)) {
    return nullptr;
  }

  ifExp->SetConsequence(ParseBlockStatement_());

  if (PeekTokenIs_(TokenType::ELSE)) {
    NextToken_();
    if (!ExpectPeek_(TokenType::LBRACE)) {
      return nullptr;
    }

    ifExp->SetAlternative(ParseBlockStatement_());
  }

  return ifExp;

}

prefixParseFn Parser::GetParseIfExpression_() {
  prefixParseFn fn = std::bind(&Parser::ParseIfExpression_, this);
  return fn;
}



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

std::shared_ptr<StringLiteral> Parser::ParseStringLiteral_() {
  auto sl = std::make_shared<StringLiteral>(curr_token_, curr_token_->GetLiteral());
  return sl;
}

prefixParseFn Parser::GetParseStringLiteralFn_() {
  prefixParseFn fn = std::bind(&Parser::ParseStringLiteral_, this);
  return fn;
}


