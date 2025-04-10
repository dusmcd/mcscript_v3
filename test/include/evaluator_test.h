#ifndef MCSCRIPT_V3_TEST_EVALUATOR_TEST_H
#define MCSCRIPT_V3_TEST_EVALUATOR_TEST_H

#include <string>

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
};



#endif // MCSCRIPT_V3_TEST_EVALUATOR_TEST_H