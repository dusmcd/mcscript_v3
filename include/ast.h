#ifndef MCSCRIPT_V3_AST_H
#define MCSCRIPT_V3_AST_H

#include <string>
#include <vector>
#include <memory>
#include <token.h>

// Node interface
class Node {
  public:
    virtual std::string TokenLiteral() const = 0;
    virtual std::string String() const = 0;
};

// Statement interface
class Statement : public Node {
  public:
    virtual std::string TokenLiteral() const = 0;
    virtual std::string String() const = 0;
  protected:
    virtual void StatementNode_() const = 0;
};

// Expression interface
class Expression : public Node {
  public:
    virtual std::string TokenLiteral() const = 0;
    virtual std::string String() const = 0;
  protected:
    virtual void ExpressionNode_() const = 0;
};

class Program : public Node {
  public:
    inline std::string TokenLiteral() const override {
      return statements_[0]->TokenLiteral();
    }
    inline std::vector<std::shared_ptr<Statement>> GetStatements() const {
      return statements_;
    }

    std::string String() const override;

    inline void AppendStatements(std::shared_ptr<Statement> stmt) {
      statements_.push_back(stmt);
    }

  private:
    std::vector<std::shared_ptr<Statement>> statements_;
};

class Identifier : public Expression {
  public:
    Identifier(std::string value, std::shared_ptr<Token> token) : 
        value_(value), token_(token) {
        // empty
    }

    inline std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    inline std::string GetValue() const {
      return value_;
    }

    inline std::shared_ptr<Token> GetToken() const {
      return token_;
    }

    std::string String() const override;
  
  protected:
    inline void ExpressionNode_() const override {}
  
  private:
    std::string value_;
    std::shared_ptr<Token> token_;

};

class ExpressionStatement : public Statement {
  public:
    ExpressionStatement(std::shared_ptr<Token> token) : token_(token) {}

    inline std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    inline std::shared_ptr<Expression> GetExpression() const {
      return expression_;
    }

    inline void SetExpression(std::shared_ptr<Expression> expr) {
      expression_ = expr;
    }

    std::string String() const override;

  protected:
    void StatementNode_() const override {}

  private:
    std::shared_ptr<Token> token_;
    std::shared_ptr<Expression> expression_;
};


class VarStatement : public Statement {
  public:
    VarStatement(std::shared_ptr<Token> token) : token_(token) {}

    inline std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    std::string String() const override;

    inline std::shared_ptr<Token> GetToken() const {
      return token_;
    }

    inline std::shared_ptr<Expression> GetValue() const {
      return value_;
    }
    inline std::shared_ptr<Identifier> GetName() const {
      return name_;
    }

    inline void SetValue(std::shared_ptr<Expression> val) {
      value_ = val;
    }

    inline void SetName(std::shared_ptr<Identifier> name) {
      name_ = name;
    }
  
  protected:
    inline void StatementNode_() const override { 
      // empty
    }
  
  private:
    std::shared_ptr<Token> token_;
    std::shared_ptr<Expression> value_;
    std::shared_ptr<Identifier> name_;
    
};

class ReturnStatement : public Statement {
  public:
    ReturnStatement(std::shared_ptr<Token> tok) : token_(tok) {}

    inline std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    std::string String() const override;

    inline std::shared_ptr<Expression> GetReturnVal() const {
      return return_value_;
    }
  
  protected:
    inline void StatementNode_() const override {}
  

  private:
    std::shared_ptr<Token> token_;
    std::shared_ptr<Expression> return_value_;
};

class IntegerLiteral : public Expression {
  public:
    IntegerLiteral(std::shared_ptr<Token> token) : token_(token) {}

    inline std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    inline std::string String() const override {
      return token_->GetLiteral();
    }

    inline long GetValue() const {
      return value_;
    }

    inline void SetValue(long value) {
      value_ = value;
    }

  protected:
    void ExpressionNode_() const override {}

  private:
    std::shared_ptr<Token> token_;
    long value_;
  
    
};

class PrefixExpression : public Expression {
  public:
    PrefixExpression(std::shared_ptr<Token> token, std::string op) : token_(token), op_(op) {}

    inline std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    std::string String() const override;

    inline std::shared_ptr<Expression> GetRight() const {
      return right_;
    }

    inline void SetRight(std::shared_ptr<Expression> right) {
      right_ = right;
    }

  protected:
    inline void ExpressionNode_() const override {}

  private:
    std::shared_ptr<Token> token_;
    std::string op_;
    std::shared_ptr<Expression> right_;

};
#endif // MCSCRIPT_V3_AST_H