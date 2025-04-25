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
  TestIfElseEvals_();
  TestReturnStmtEvals_();
  TestErrorMessages_();
  TestIdentifierEvals_();
  TestFunctionLiterals_();
}

/*
================================================
PRIVATE METHODS
================================================
*/

/*
  helper methods
*/

bool EvaluatorTest::TestBooleanObject_(Object* obj, bool expected) {
  auto boolean = dynamic_cast<Boolean*>(obj);
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


Object* EvaluatorTest::TestEval_(std::string input) {
    auto l = std::make_shared<Lexer>(input);
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();
    auto env = std::make_shared<Environment>();

    Object* obj = evaluator_.Eval(program, env);
    evaluator_.TrackObject(obj);

    return obj;
}

/*
  main test methods
*/

void EvaluatorTest::TestFunctionLiterals_() {
  std::string input = "function(x) { x; };";

  Object* obj = TestEval_(input);
  auto fn = dynamic_cast<Function*>(obj);
  if (fn == nullptr) {
    std::cerr << "obj is not a Function\n";
    return;
  }

  if (fn->GetParams().size() != 1) {
    std::cerr << "wrong number of function params. expected: " <<
      1 << ", got: " << fn->GetParams().size() << "\n";
    return;
  }

  if (fn->GetParams()[0]->GetValue().compare("x") != 0) {
    std::cerr << "function parameters are wrong, expected: x, got: "
        << fn->GetParams()[0]->GetValue() << "\n";
    return;
  }

  if (fn->GetBody()->String().compare("x")) {
    std::cerr << "function body wrong. expected: x, got : " 
        << fn->GetBody()->String() << "\n";
    return;
  }

  evaluator_.CollectGarbage();
  std::cout << "TestFunctionLiterals_() passed\n";
}

void EvaluatorTest::TestIdentifierEvals_() {
  std::vector<IntegerTest> tests = {
    (IntegerTest){.input = "var a = 5; a;", .expectedVal = 5},
    (IntegerTest){.input = "var a = 10; var b = a; b;", 10},
    (IntegerTest){.input = "var a = 5; var b = 5; var c = a + b + 5; c;", 15},
    (IntegerTest){.input = "var a = 5; a; a; a;", .expectedVal = 5}
  };

  for (const auto& test : tests) {
    Object* evaluated = TestEval_(test.input);
    if (!TestIntegerObject_(evaluated, test.expectedVal)) {
      return;
    }
  }

  evaluator_.CollectGarbage();
  std::cout << "TestIdentifierEvals_() passed\n";
}

void EvaluatorTest::TestErrorMessages_() {
  std::vector<ErrorTest> tests = {
    (ErrorTest){
      .input = "true + true;",
      .expectedVal = "unknown operator: BOOLEAN + BOOLEAN",
    },
    (ErrorTest){
      .input = "5 + true;",
      .expectedVal = "unknown operator: INTEGER + BOOLEAN",
    },
    (ErrorTest){
      .input = "-true;",
      .expectedVal = "unknown operator: -BOOLEAN" 
    },
    (ErrorTest){
      .input = "true + false; 10;",
      .expectedVal = "unknown operator: BOOLEAN + BOOLEAN"
    },
    (ErrorTest){
      .input = "foobar;",
      .expectedVal = "unexpected identifier: foobar"
    }
  };

  for (const auto& test : tests) {
    Object* obj = TestEval_(test.input);
    auto err = dynamic_cast<Error*>(obj);

    if (err == nullptr) {
      std::cerr << "obj is not an Error*\n";
      return;
    }

    std::string msg = err->GetMessage();
    if (msg.compare(test.expectedVal) != 0) {
      std::cerr << "wrong message. expected: " << test.expectedVal
        << ", got: " << msg << "\n";
      return;
    }
  }

  evaluator_.CollectGarbage();

  std::cout << "TestErrorMessages_() passed\n";
}

void EvaluatorTest::TestReturnStmtEvals_() {
  std::vector<IntegerTest> tests = {
    (IntegerTest){.input = "return 10;", .expectedVal = 10},
    (IntegerTest){.input = "return 10; 9;", .expectedVal = 10},
    (IntegerTest){.input = "5 + 2; return 10; 9;", .expectedVal = 10},
    (IntegerTest){.input =
      "if (1 < 10) {"
        "if (1 < 10) {"
          "return 10;"
        "} "
        "return 1;"
      "}", .expectedVal = 10}
  };

  for (const auto& test : tests) {
    Object* obj = TestEval_(test.input);

    if (!TestIntegerObject_(obj, test.expectedVal)) {
      return;
    }
  }

  evaluator_.CollectGarbage();
  std::cout << "TestReturnStmtEvals_() passed\n";
}

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
    Object* obj = TestEval_(test.input);

    if (!TestBooleanObject_(obj, test.expectedVal)) {
      return;
    }
  }

  evaluator_.CollectGarbage();
  std::cout << "TestBangOperatorEvals_() passed\n";
}

void EvaluatorTest::TestBooleanEvals_() {
  std::vector<BooleanTest> tests = {
    (BooleanTest){.input = "true;", .expectedVal = true},
    (BooleanTest){.input = "false;", .expectedVal = false},
    (BooleanTest){.input = "1 > 2", .expectedVal = false},
    (BooleanTest){.input = "1 < 2", .expectedVal = true},
    (BooleanTest){.input = "1 == 1", .expectedVal = true},
    (BooleanTest){.input = "1 != 2", .expectedVal = true},
    (BooleanTest){.input = "true == true", .expectedVal = true},
    (BooleanTest){.input = "true == false", .expectedVal = false},
    (BooleanTest){.input = "(1 < 2) == true", .expectedVal = true}
  };

  for (const auto& test : tests) {
    Object* obj = TestEval_(test.input);

    if (!TestBooleanObject_(obj, test.expectedVal)) {
      return;
    }

  }

  evaluator_.CollectGarbage();
  std::cout << "TestBooleanEvals_() passed\n";
}

void EvaluatorTest::TestIntegerEvals_() {
  std::vector<IntegerTest> tests = {
    (IntegerTest){.input = "5;", .expectedVal = 5},
    (IntegerTest){.input = "15;", .expectedVal = 15},
    (IntegerTest){.input = "10000;", .expectedVal = 10000},
    (IntegerTest){.input = "-5", .expectedVal = -5},
    (IntegerTest){.input = "-10", .expectedVal = -10},
    (IntegerTest){.input = "5 + 5", .expectedVal = 10},
    (IntegerTest){.input = "5 * 5", .expectedVal = 25},
    (IntegerTest){.input = "10 - 5", .expectedVal = 5},
    (IntegerTest){.input = "10 / 5", .expectedVal = 2}
  };

  for (const auto& test : tests) {
    Object* obj = TestEval_(test.input);

    if (!TestIntegerObject_(obj, test.expectedVal)) {
      return;
    }
  }

  evaluator_.CollectGarbage();
  std::cout << "TestIntegerEvals_() passed\n";
}

void EvaluatorTest::TestIfElseEvals_() {
  std::vector<IfTest> tests = {
    (IfTest){.input = "if (true) { 10 }", .expectedVal = 10},
    (IfTest){.input = "if (false) { 10 }", .expectedVal = nullptr},
    (IfTest){.input = "if (1 < 2) { 10 } else { 20 }", .expectedVal = 10},
    (IfTest){.input = "if (1 > 2) { 10 } else { 20 }", .expectedVal = 20}
  };

  for (const auto& test : tests) {
    Object* obj = TestEval_(test.input);

    if (std::holds_alternative<long>(test.expectedVal)) {
      if (!TestIntegerObject_(obj, std::get<long>(test.expectedVal))) {
        return;
      }
    } else if (!TestNullObject_(obj)) {
      return;
    }
  }

  evaluator_.CollectGarbage();
  std::cout << "TestIfElseEvals_() passed\n";
}

bool EvaluatorTest::TestIntegerObject_(Object* obj, long expected) {
  auto integer = dynamic_cast<Integer*>(obj);
  if (integer == nullptr) {
    std::cerr << "obj is not an Integer\n";
    return false;
  }

  if (integer->GetValue() != expected) {
    std::cerr << "wrong value. expected: " 
        << expected << ", got: " << integer->GetValue()
        << "\n";
    return false;
  }

  return true;
}

bool EvaluatorTest::TestNullObject_(Object* obj) {
  if (obj != evaluator_.NULL_T()) {
    std::cerr << "obj is not NULL_T\n";
    return false;
  }

  return true;
}


int main() {
  GCollector& gCollector = GCollector::getGCollector();
  Boolean* TRUE = new Boolean(true);
  Boolean* FALSE = new Boolean(false);
  Null* NULL_T = new Null();
  Evaluator evaluator(gCollector, TRUE, FALSE, NULL_T);
  EvaluatorTest eTest(evaluator);

  eTest.Run();

  return 0;
}