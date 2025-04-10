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
}

/*
================================================
PRIVATE METHODS
================================================
*/

/*
  main test methods
*/

void EvaluatorTest::TestBooleanEvals_() {
  std::vector<BooleanTest> tests = {
    (BooleanTest){.input = "true;", .expectedVal = true},
    (BooleanTest){.input = "false;", .expectedVal = false}
  };

  for (const auto& test : tests) {
    auto l = std::make_shared<Lexer>(test.input);
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();

    std::shared_ptr<Object> obj = Eval(program);

    auto boolean = std::dynamic_pointer_cast<Boolean>(obj);
    if (obj == nullptr){
      std::cerr << "obj is not a Boolean\n";
      return;
    }

    if (boolean->GetValue() != test.expectedVal) {
      std::cerr << "boolean value wrong. expected: " << test.expectedVal
          << ", got: " << boolean->GetValue() << "\n";
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
    auto l = std::make_shared<Lexer>(test.input);
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();

    std::shared_ptr<Object> obj = Eval(program);

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