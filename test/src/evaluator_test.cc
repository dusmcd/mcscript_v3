#include <evaluator_test.h>
#include <parser.h>
#include <evaluator.h>
#include <iostream>

/*
=================================================
PUBLIC METHODS
=================================================
*/

void EvaluatorTest::Run() {
  TestIntegerEvals_();
  TestBooleanEvals_();
  TestBangOperatorEvals_();
}

/*
================================================
PRIVATE METHODS
================================================
*/

/*
  helper methods
*/

bool EvaluatorTest::TestBooleanObject_(std::shared_ptr<Object> obj, bool expected) {
  auto boolean = std::dynamic_pointer_cast<Boolean>(obj);
  if (boolean == nullptr){
    std::cerr << "obj is not a Boolean\n";
    return false;
  }

  if (boolean->GetValue() != expected) {
    std::cerr << "boolean value wrong. expected: " << expected
        << ", got: " << boolean->GetValue() << "\n";
    return false;
  }

  return true;

}


std::shared_ptr<Object> EvaluatorTest::TestEval_(std::string input) {
    auto l = std::make_shared<Lexer>(input);
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();

    std::shared_ptr<Object> obj = Eval(program);

    return obj;
}

/*
  main test methods
*/


void EvaluatorTest::TestBangOperatorEvals_() {
  std::vector<BooleanTest> tests = {
    (BooleanTest){.input = "!true", .expectedVal = false},
    (BooleanTest){.input = "!false", .expectedVal = true},
    (BooleanTest){.input = "!5", .expectedVal = false},
    (BooleanTest){.input = "!!true", .expectedVal = true},
    (BooleanTest){.input = "!!false", .expectedVal = false},
    (BooleanTest){.input = "!!5", .expectedVal = true}
  };

  for (const auto& test : tests) {
    std::shared_ptr<Object> obj = TestEval_(test.input);

    if (!TestBooleanObject_(obj, test.expectedVal)) {
      return;
    }
  }

  std::cout << "TestBangOperatorEvals_() passed\n";
}

void EvaluatorTest::TestBooleanEvals_() {
  std::vector<BooleanTest> tests = {
    (BooleanTest){.input = "true;", .expectedVal = true},
    (BooleanTest){.input = "false;", .expectedVal = false}
  };

  for (const auto& test : tests) {
    std::shared_ptr<Object> obj = TestEval_(test.input);

    if (!TestBooleanObject_(obj, test.expectedVal)) {
      return;
    }

  }

  std::cout << "TestBooleanEvals_() passed\n";
}

void EvaluatorTest::TestIntegerEvals_() {
  std::vector<IntegerTest> tests = {
    (IntegerTest){.input = "5;", .expectedVal = 5},
    (IntegerTest){.input = "15;", .expectedVal = 15},
    (IntegerTest){.input = "10000;", .expectedVal = 10000}
  };

  for (const auto& test : tests) {
    std::shared_ptr<Object> obj = TestEval_(test.input);

    auto integer = std::dynamic_pointer_cast<Integer>(obj);
    if (integer == nullptr) {
      std::cerr << "obj is not an Integer\n";
      return;
    }

    if (integer->GetValue() != test.expectedVal) {
      std::cerr << "wrong value. expected: " 
          << test.expectedVal << ", got: " << integer->GetValue()
          << "\n";
      return;
    }
  }

  std::cout << "TestIntegerEvals_() passed\n";
}

int main() {
  EvaluatorTest eTest = EvaluatorTest();
  eTest.Run();

  return 0;
}