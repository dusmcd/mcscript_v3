#ifndef MCSCRIPT_V3_TEST_PARSER_TEST_H
#define MCSCRIPT_V3_TEST_PARSER_TEST_H

#include <parser.h>
#include <ast.h>
#include <iostream>
#include <variant>

using expressionVal = std::variant<long, bool, std::string>;

struct Test {
  std::string literal;
};

struct InfixTest {
  std::string input;
  expressionVal left;
  std::string op;
  expressionVal right;
};

struct PrefixTest {
  expressionVal value;
  std::string op;
  std::string input;
};

struct FunctionTest {
  std::string input;
  std::vector<std::string> expectedParams;
  expressionVal body;
};

struct CallTest {
  std::string input;
  std::vector<expressionVal> expectedParams;
};

class ParserTest {
  public:
    void Run();

  private:
    
    // helper methods
    bool CheckParserErrors_(std::shared_ptr<Parser> p);
    bool TestIntegerLiteral_(std::shared_ptr<Expression> exp, expressionVal value);
    bool TestVarStatement_(std::shared_ptr<VarStatement> vs, Test test);
    bool TestIdentityExpression_(std::shared_ptr<Expression> exp, expressionVal value);
    bool TestBooleanExpression_(std::shared_ptr<Expression> exp, expressionVal value);
    bool TestBlockStatement_(std::shared_ptr<BlockStatement> block, size_t size, expressionVal value);
    
    bool TestInfixExpression_(
      std::shared_ptr<Expression> exp,
      expressionVal left,
      std::string op,
      expressionVal right
    );
    
    bool TestLiteralExpression_(std::shared_ptr<Expression> exp, expressionVal value);

    // main test methods
    void TestArrayLiterals_();
    void TestInfixExpressions_();
    void TestPrefixExpressions_();
    void TestIntegerLiterals_();
    void TestIdentityExpressions_();
    void TestString_();
    void TestVarStatements_();
    void TestReturnStatements_();
    void TestOperatorPrecedence_();
    void TestIfExpression_();
    void TestIfElseExpression_();
    void TestFunctionLiteral_();
    void TestCallExpressions_();
    void TestStringLiteral_();

};



#endif //MCSCRIPT_V3_TEST_PARSER_TEST_H
