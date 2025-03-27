#ifndef MCSCRIPT_V3_TEST_PARSER_TEST_H
#define MCSCRIPT_V3_TEST_PARSER_TEST_H

#include <parser.h>
#include <ast.h>
#include <iostream>

struct Test {
  std::string literal;
};

template <typename T>
struct InfixTest {
  std::string input;
  T left;
  std::string op;
  T right;
};

template <typename T>
struct PrefixTest {
  T value;
  std::string op;
  std::string input;
};


class ParserTest {
  public:
    void Run();

  private:
    
    // helper methods
    bool CheckParserErrors_(std::shared_ptr<Parser> p);
    bool TestIntegerLiteral_(std::shared_ptr<Expression> exp, long value);
    bool TestVarStatement_(std::shared_ptr<VarStatement> vs, Test test);
    bool TestIdentityExpression_(std::shared_ptr<Expression> exp, std::string value);
    bool TestBooleanExpression_(std::shared_ptr<Expression> exp, bool value);
    
    template <typename T>
    bool TestInfixExpression_(
      std::shared_ptr<Expression> exp,
      T left,
      std::string op,
      T right
    );
    
    template <typename T>
    bool TestLiteralExpression_(std::shared_ptr<Expression> exp, T value);

    // main test methods
    void TestInfixExpressions_();
    void TestPrefixExpressions_();
    void TestIntegerLiterals_();
    void TestIdentityExpressions_();
    void TestString_();
    void TestVarStatements_();
    void TestReturnStatements_();
    void TestOperatorPrecedence_();
    void TestIfExpression_();

};



#endif //MCSCRIPT_V3_TEST_PARSER_TEST_H
