#ifndef MCSCRIPT_V3_TEST_PARSER_TEST_H
#define MCSCRIPT_V3_TEST_PARSER_TEST_H

#include <parser.h>
#include <ast.h>
#include <iostream>

struct Test {
    std::string literal;
};

struct PrefixTest {
  long value;
  std::string op;
  std::string input;
};


class ParserTest {
  public:
    void Run();

  private:
    
    // helper methods
    bool CheckParserErrors_(std::shared_ptr<Parser> p);
    bool TestIntegerLiteral_(std::shared_ptr<IntegerLiteral> il, int value);
    bool TestVarStatement_(std::shared_ptr<VarStatement> vs, Test test);

    // main test methods
    void TestInfixExpressions_();
    void TestPrefixExpressions_();
    void TestIntegerLiterals_();
    void TestIdentityExpressions_();
    void TestString_();
    void TestVarStatements_();
    void TestReturnStatements_();

};



#endif //MCSCRIPT_V3_TEST_PARSER_TEST_H
