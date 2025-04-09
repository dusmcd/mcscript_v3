#ifndef MCSCRIPT_V3_TEST_EVALUATOR_TEST_H
#define MCSCRIPT_V3_TEST_EVALUATOR_TEST_H

#include <string>

struct IntegerTest {
  std::string input;
  long expectedVal;
};

class EvaluatorTest {
  public:
    void Run();

  private:
    void TestIntegerEvals_();
};



#endif // MCSCRIPT_V3_TEST_EVALUATOR_TEST_H