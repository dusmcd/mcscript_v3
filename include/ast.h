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

    inline void SetReturnVal(std::shared_ptr<Expression> exp) {
      return_value_ = exp;
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

class InfixExpression : public Expression {
  public:
    InfixExpression(std::shared_ptr<Token> token, std::string op, 
              std::shared_ptr<Expression> left) : token_(token), left_(left), op_(op) {
                // empty
    }

    inline std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    inline std::shared_ptr<Expression> GetLeft() const {
      return left_;
    }

    inline std::shared_ptr<Expression> GetRight() const {
      return right_;
    }

    inline std::string GetOp() const {
      return op_;
    }


    inline void SetRight(std::shared_ptr<Expression> right) {
      right_ = right;
    }

    std::string String() const override;

  protected:
    void ExpressionNode_() const override {}
  
  private:
    std::shared_ptr<Token> token_;
    std::shared_ptr<Expression> left_;
    std::string op_;
    std::shared_ptr<Expression> right_;
};

class BooleanExpression : public Expression {
  public:
    BooleanExpression(std::shared_ptr<Token> token, bool value) : token_(token), value_(value) {
      // empty
    }

    inline std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    inline std::string String() const override {
      return token_->GetLiteral();
    }

    inline bool GetValue() const {
      return value_;
    }
  
  protected:
    void ExpressionNode_() const override {}
  
  private:
    std::shared_ptr<Token> token_;
    bool value_;
};

class BlockStatement : public Statement {
  public:
    BlockStatement(std::shared_ptr<Token> token) : token_(token) {
      // empty
    }

    inline std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    inline void AppendStatements(std::shared_ptr<Statement> stmt) {
      statements_.push_back(stmt);
    }

    inline std::vector<std::shared_ptr<Statement>> GetStatements() const {
      return statements_;
    }

    std::string String() const override;

  protected:
    void StatementNode_() const override {}

  private:
    std::shared_ptr<Token> token_;
    std::vector<std::shared_ptr<Statement>> statements_;
};

class ForStatement : public Statement {
  public:
    ForStatement(
        std::shared_ptr<Token> tok,
        std::shared_ptr<VarStatement> varStmt,
        std::shared_ptr<Expression> condition,
        std::shared_ptr<Expression> afterAction,
        std::shared_ptr<BlockStatement> block
        ) : tok_(tok), varStmt_(varStmt), condition_(condition),
            afterAction_(afterAction), block_(block) {
        // empty
    }

    inline std::string TokenLiteral() const override {
      return tok_->GetLiteral();
    }

    inline std::shared_ptr<VarStatement> GetVarStmt() const {
      return varStmt_;
    }

    inline std::shared_ptr<Expression> GetCondition() const {
      return condition_;
    }

    inline std::shared_ptr<Expression> GetAfterAction() const {
      return afterAction_;
    }

    inline std::shared_ptr<BlockStatement> GetBlock() const {
      return block_;
    }

    std::string String() const override;

  protected:
    void StatementNode_() const override {}

  private:
    std::shared_ptr<Token> tok_;
    std::shared_ptr<VarStatement> varStmt_;
    std::shared_ptr<Expression> condition_;
    std::shared_ptr<Expression> afterAction_;
    std::shared_ptr<BlockStatement> block_;
};


class IfExpression : public Expression {
  public:
    IfExpression(std::shared_ptr<Token> token) : token_(token) {
      alternative_ = nullptr;
    }

    inline std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    inline std::shared_ptr<Expression> GetCondition() const {
      return condition_;
    }

    inline std::shared_ptr<BlockStatement> GetConsequence() const {
      return consequence_;
    }

    inline std::shared_ptr<BlockStatement> GetAlternative() const {
      return alternative_;
    }

    std::string String() const override;

    inline void SetCondition(std::shared_ptr<Expression> exp) {
      condition_ = exp;
    }

    inline void SetConsequence(std::shared_ptr<BlockStatement> bs) {
      consequence_ = bs;
    }

    inline void SetAlternative(std::shared_ptr<BlockStatement> bs) {
      alternative_ = bs;
    }

  protected:
    void ExpressionNode_() const override {}
  
  private:
    std::shared_ptr<Token> token_;
    std::shared_ptr<Expression> condition_;
    std::shared_ptr<BlockStatement> consequence_;
    std::shared_ptr<BlockStatement> alternative_;
};

class FunctionLiteral : public Expression {
  public:
    FunctionLiteral(std::shared_ptr<Token> token) : token_(token) {
      // empty
    }

    inline std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    inline std::vector<std::shared_ptr<Identifier>> GetParameters() const {
      return parameters_;
    }

    inline std::shared_ptr<BlockStatement> GetBody() const {
      return body_;
    }

    inline void SetParameters(std::vector<std::shared_ptr<Identifier>> params) {
      parameters_ = params;
    }

    inline void SetBody(std::shared_ptr<BlockStatement> block) {
      body_ = block;
    }

    std::string String() const override;

  protected:
    void ExpressionNode_() const override {}

  private:
    std::shared_ptr<Token> token_;
    std::vector<std::shared_ptr<Identifier>> parameters_;
    std::shared_ptr<BlockStatement> body_;
};

class CallExpression : public Expression {
  public:
    CallExpression(std::shared_ptr<Token> tok, std::shared_ptr<Expression> func) : token_(tok), func_(func) {
      //empty
    }

    std::string TokenLiteral() const override {
      return token_->GetLiteral();
    }

    std::string String() const;

    inline void SetArgs(std::vector<std::shared_ptr<Expression>> args) {
      args_ = args;
    }

    inline std::vector<std::shared_ptr<Expression>> GetArgs() const {
      return args_;
    }

    inline std::shared_ptr<Expression> GetFunc() const {
      return func_;
    }

  protected:
    void ExpressionNode_() const override {}

  private:
    std::shared_ptr<Token> token_;
    std::shared_ptr<Expression> func_;
    std::vector<std::shared_ptr<Expression>> args_;
};

class StringLiteral : public Expression {
  public:
    StringLiteral(std::shared_ptr<Token> tok, std::string literal) : tok_(tok), literal_(literal) {
      // empty
    }

    inline std::string TokenLiteral() const override {
      return literal_;
    }

    inline std::string String() const override {
      return literal_;
    }

  protected:
    void ExpressionNode_() const override {}
  
  private:
    std::shared_ptr<Token> tok_;
    std::string literal_;
};

class ArrayLiteral : public Expression {
  public:
    ArrayLiteral(std::shared_ptr<Token> tok) : 
      tok_(tok) {
        // empty
      }

    inline void SetExps(std::vector<std::shared_ptr<Expression>> exps) {
      exps_ = exps;
    }

    inline std::string TokenLiteral() const override{
      return tok_->GetLiteral();
    }

    inline std::vector<std::shared_ptr<Expression>> GetExps() const {
      return exps_;
    }

    std::string String() const override;

  protected:
    void ExpressionNode_() const override {}



  private:
    std::shared_ptr<Token> tok_;
    std::vector<std::shared_ptr<Expression>> exps_;
};

class IndexExpression : public Expression {
  public:
    IndexExpression(std::shared_ptr<Expression> exp) : exp_(exp) {
      // empty
    }

    inline std::shared_ptr<Expression> GetIdx() const {
      return idx_;
    }

    inline void SetIdx(std::shared_ptr<Expression> idx) {
      idx_ = idx;
    }

    inline std::shared_ptr<Expression> GetExp() const {
      return exp_;
    }

    inline std::string TokenLiteral() const override {
      return tok_->GetLiteral();
    }

    std::string String() const override;


  protected:
    void ExpressionNode_() const override {}

  private:
    std::shared_ptr<Token> tok_;
    std::shared_ptr<Expression> idx_;
    std::shared_ptr<Expression> exp_;
};

class AssignExpression : public Expression {
  public:
    AssignExpression(std::shared_ptr<Expression> ident) : ident_(ident) {
      // empty
    }

    inline std::string TokenLiteral() const override {
      return tok_->GetLiteral();
    }

    inline void SetNewVal(std::shared_ptr<Expression> newVal) {
      newVal_ = newVal;
    }

    inline std::shared_ptr<Expression> GetIdent() const {
      return ident_;
    }

    inline std::shared_ptr<Expression> GetNewVal() const {
      return newVal_;
    }

    std::string String() const override;

  protected:
    void ExpressionNode_() const override {}

  private:
    std::shared_ptr<Token> tok_;
    std::shared_ptr<Expression> ident_;
    std::shared_ptr<Expression> newVal_;

};


#endif // MCSCRIPT_V3_AST_H
