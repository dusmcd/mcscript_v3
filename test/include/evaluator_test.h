#ifndef MCSCRIPT_V3_TEST_EVALUATOR_TEST_H
#define MCSCRIPT_V3_TEST_EVALUATOR_TEST_H

#include <string>
#include <memory>
#include <object.h>
#include <evaluator.h>
#include <variant>

using ifVal = std::variant<long, void*, bool>;

struct IfTest {
  std::string input;
  ifVal expectedVal;
};

struct IntegerTest {
  std::string input;
  long expectedVal;
};

struct BooleanTest {
  std::string input;
  bool expectedVal;
};

struct ErrorTest {
  std::string input;
  std::string expectedVal;
};

class EvaluatorTest {
  public:
    void Run();
    EvaluatorTest(Evaluator& evaluator) : evaluator_(evaluator) {
      // empty
    }

  private:
    void TestIntegerEvals_();
    void TestBooleanEvals_();
    void TestBangOperatorEvals_();
    void TestIfElseEvals_();
    void TestReturnStmtEvals_();
    void TestErrorMessages_();
    void TestIdentifierEvals_();
    void TestFunctionLiterals_();
    void TestFunctionCalls_();

    // helper methods
    Object* TestEval_(std::string input);
    bool TestBooleanObject_(Object* obj, bool expected);
    bool TestIntegerObject_(Object* obj, long expected);
    bool TestNullObject_(Object* obj);
    Evaluator& evaluator_;
    
};



#endif // MCSCRIPT_V3_TEST_EVALUATOR_TEST_H