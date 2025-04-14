#ifndef MCSCRIPT_V3_TEST_EVALUATOR_TEST_H
#define MCSCRIPT_V3_TEST_EVALUATOR_TEST_H

#include <string>
#include <memory>
#include <object.h>

struct IntegerTest {
  std::string input;
  long expectedVal;
};

struct BooleanTest {
  std::string input;
  bool expectedVal;
};

class EvaluatorTest {
  public:
    void Run();

  private:
    void TestIntegerEvals_();
    void TestBooleanEvals_();
    void TestBangOperatorEvals_();

    // helper methods
    std::shared_ptr<Object> TestEval_(std::string input);
    bool TestBooleanObject_(std::shared_ptr<Object> obj, bool expected);
};



#endif // MCSCRIPT_V3_TEST_EVALUATOR_TEST_H