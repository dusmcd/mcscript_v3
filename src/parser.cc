#include <parser.h>

void Parser::NextToken_() {
  curr_token_ = peek_token_;
  peek_token_ = l_->NextToken();
}

Parser::Parser(std::shared_ptr<Lexer> l) {
  l_ = l;
  curr_token_ = nullptr;
  peek_token_ = nullptr;

  NextToken_();
  NextToken_();
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

  // skipping expressions for now
  while (!CurrTokenIs_(TokenType::SEMICOLON)) {
    NextToken_();
  }

  return vs;
}

std::shared_ptr<ReturnStatement> Parser::ParseReturnStatement_() {
  auto rs = std::make_shared<ReturnStatement>(curr_token_);

  // skipping expressions for now
  while (!CurrTokenIs_(TokenType::SEMICOLON)) {
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
      default:
        printf("We hit default\n");
    }

  return stmt;
}

std::unique_ptr<Program> Parser::ParseProgram(){
  auto program = std::make_unique<Program>();
  
  while (!CurrTokenIs_(TokenType::EOI)) {
    std::shared_ptr<Statement> stmt = ParseStatement_();
    if (stmt != nullptr) {
      program->AppendStatements(stmt);
    }
      NextToken_();
  }
  
  return program;
}
