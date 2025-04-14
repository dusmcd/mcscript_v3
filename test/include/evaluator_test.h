#ifndef MCSCRIPT_V3_TEST_EVALUATOR_TEST_H
#define MCSCRIPT_V3_TEST_EVALUATOR_TEST_H

#include <string>
#include <memory>
#include <object.h>
#include <evaluator.h>

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
    EvaluatorTest(Evaluator& evaluator) : evaluator_(evaluator) {
      // empty
    }

  private:
    void TestIntegerEvals_();
    void TestBooleanEvals_();
    void TestBangOperatorEvals_();

    // helper methods
    Object* TestEval_(std::string input);
    bool TestBooleanObject_(Object* obj, bool expected);
    Evaluator& evaluator_;
    
};



#endif // MCSCRIPT_V3_TEST_EVALUATOR_TEST_H