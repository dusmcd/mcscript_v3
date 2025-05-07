#ifndef MCSCRIPT_V3_BUILTIN_TEST_H
#define MCSCRIPT_V3_BUILTIN_TEST_H

#include <evaluator.h>

struct IntegerTest {
  std::string input;
  long expected;
};

class BuiltInTest {
  public:
    BuiltInTest(Evaluator& evaluator) : evaluator_(evaluator) {
      // empty
    }

    void Run();

  private:
    Evaluator& evaluator_;

    // methods
    void TestLen_();
    Object* TestEval_(std::string input);

    // helpers
    bool TestIntegerObject_(Object* obj, long expected);
};




#endif //MCSCRIPT_V3_BUILTIN_TEST_H
