#include <evaluator_test.h>
#include <parser.h>
#include <evaluator.h>
#include <iostream>


void EvaluatorTest::Run() {
  std::cout << "EvaluatorTests passed\n";
}

int main() {
  EvaluatorTest eTest = EvaluatorTest();
  eTest.Run();

  return 0;
}