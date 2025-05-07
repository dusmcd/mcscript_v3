#include <evaluator_test.h>
#include <memory>
#include <parser.h>
#include <evaluator.h>
#include <iostream>
#include <unordered_map>

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
  TestFunctionCalls_();
  TestClosures_();
  TestGCollector_();
  TestStrings_();
  TestStringConcat_();
  TestArrays_();
  TestIndexExps_();
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
    auto env = std::make_shared<Environment<Object*>>();

    Object* obj = evaluator_.Eval(program, env);
    return obj;
}

/*
  main test methods
*/


void EvaluatorTest::TestIndexExps_() {
  std::vector<IntegerTest> tests = {
    (IntegerTest){.input = "var arr = [1, 2, 3]; arr[0]", .expectedVal = 1},
    (IntegerTest){.input = "var arr = [2, 3, 4]; arr[2];", .expectedVal = 4},
    (IntegerTest){.input = "var arr = [1, 2, 3]; arr[3];", .expectedVal = -1},
    (IntegerTest){.input = "var arr = [1, 2, 3]; arr[-1];", .expectedVal = -1}
  };

  for (size_t i = 0; i < 2; i++) {
    if (!TestIntegerObject_(TestEval_(tests[i].input), tests[i].expectedVal)) {
      return;
    }
  }

  Object* objNull1 = TestEval_(tests[2].input);

  if (objNull1 != evaluator_.NULL_T()) {
    std::cerr << "expected object to equal NULL. got=" << objNull1->Inspect()
      << "\n";
    return;
  }

  Object* objNull2 = TestEval_(tests[3].input);
  if (objNull2 != evaluator_.NULL_T()) {
    std::cerr << "expected object to equal NULL. got =" <<
      objNull2->Inspect() << "\n";
    return;
  }

  evaluator_.FinalCleanup();
  std::cout << "TestIndexExps_() passed\n";
}

void EvaluatorTest::TestArrays_() {
  std::vector<ArrayTest> tests = {
    (ArrayTest){.input = "[1, 2, 3]", .expected = std::vector<long>{1, 2, 3}},
    (ArrayTest){.input = "[1 + 2, 3, 4]", .expected = std::vector<long>{1 + 2, 3, 4}}
  };
  
  for (const auto& test : tests) {
    Object* obj = TestEval_(test.input);

    auto arr = dynamic_cast<Array*>(obj);
    if (arr == nullptr) {
      std::cerr << "object is not an Array\n";
      return;
    }

    std::shared_ptr<std::vector<Object*>> elements = arr->GetElements();
    for (size_t i = 0; i < test.expected.size(); i++) {
      if (!TestIntegerObject_((*elements)[i], test.expected[i])) {
        return;
      }
    }
  }

  evaluator_.FinalCleanup();
  std::cout << "TestArrays_() passed\n";
}

void EvaluatorTest::TestStringConcat_() {
  std::string input = "\"hello\" + \" world\"";

  Object* obj = TestEval_(input);
  auto str = dynamic_cast<String*>(obj);
  if (str == nullptr) {
    std::cerr << "object is not a String\n";
    return;
  }

  if (str->GetValue().compare("hello world") != 0) {
    std::cerr << "str value wrong. expected: \"hello world\""
        << ", got: " << str->GetValue() << "\n";
    return;
  }

  evaluator_.FinalCleanup();
  std::cout << "TestStringConat_() passed\n";
}

void EvaluatorTest::TestStrings_() {
  StringTest test = {.input = "\"Hello World!\"", .expectedVal = "Hello World!"};
  Object* obj = TestEval_(test.input);

  auto str = dynamic_cast<String*>(obj);
  if (str == nullptr) {
    std::cerr << "object is not a String\n";
    return;
  }

  if (str->GetValue().compare(test.expectedVal) != 0) {
    std::cerr << "str->GetValue() wrong. expected: " 
        << test.expectedVal << ", got: " << str->GetValue() << "\n";
    return;
  }

  evaluator_.FinalCleanup();
  std::cout << "TestStrings_() passed\n";
}

void EvaluatorTest::TestGCollector_() {
  std::vector<CollectorTest> tests = {
    (CollectorTest){.input = "var x = 1 + 2;", .before = 3, .after = 1},
    (CollectorTest){.input = "var a = 3; var b = 5 + a;", .before = 3, .after = 2},
    (CollectorTest){.input = "var a = 3; var b = 1 + 2 + a;", .before = 5, .after = 2},
    (CollectorTest){.input = "var add = function(a, b) { a + b; }; var sum = add(1, 2)", .before = 4, .after = 2},
    (CollectorTest){.input = "var str = \"something\"; var foobar = str + \"hello\";", .before = 3, .after = 2},
    (CollectorTest){.input = "var arr = [1, 2, 3]", .before = 4, .after = 4}
  };

  for (const auto& test : tests) {
    TestEval_(test.input);
    size_t numObjects = evaluator_.GetNumObjects();
    if (numObjects != test.before) {
      std::cerr << "number of objects before garbage collection wrong. expected: " 
          << test.before << ", got: " << numObjects << "\n";
      return;
    }
    evaluator_.CollectGarbage();

    numObjects = evaluator_.GetNumObjects();
    if (numObjects != test.after) {
      std::cerr << "number of objects after garbage collection wrong. expected: "
        << test.after << ", got: " << numObjects << "\n";
      return;
    }

    evaluator_.FinalCleanup();
  }

  std::cout << "TestGCollector_() passed\n";
}


void EvaluatorTest::TestClosures_() {
  std::string input = 
  "var newAdder = function(x) {"
  " return function(a) {"
      "return a + x;"
      "}"
    "};"
  "var addTwo = newAdder(2);"
  "addTwo(5);";

  Object* obj = TestEval_(input);
  if (!TestIntegerObject_(obj, 7)) {
    return;
  }

  evaluator_.FinalCleanup();
  std::cout << "TestClosures_() passed\n";
}

void EvaluatorTest::TestFunctionCalls_() {
  std::vector<IntegerTest> tests = {
    (IntegerTest){.input = "var add = function(a, b) { return a + b; }; add(5, 5);", .expectedVal = 10},
    (IntegerTest){.input = "var add = function(a, b) { a + b; }; add(5, 5);", .expectedVal = 10},
    (IntegerTest){.input = "var add = function(a, b) { return a + b; }; add(5 +5, 10);", .expectedVal = 20},
    (IntegerTest){.input = "var x = 5; var doub = function(a) { return a * 2; }; doub(x);", .expectedVal = 10}
  };

  for (const auto& test : tests) {
    Object* obj = TestEval_(test.input);
    if (!TestIntegerObject_(obj, test.expectedVal)) {
      return;
    }
  }

  evaluator_.FinalCleanup();
  std::cout << "TestFunctionCalls_() passed\n";
}

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

  evaluator_.FinalCleanup();
  std::cout << "TestFunctionLiterals_() passed\n";
}

void EvaluatorTest::TestIdentifierEvals_() {
  std::vector<IntegerTest> tests = {
    (IntegerTest){.input = "var a = 5; a;", .expectedVal = 5},
    (IntegerTest){.input = "var a = 10; var b = a; b;", .expectedVal = 10},
    (IntegerTest){.input = "var a = 5; var b = 5; var c = a + b + 5; c;", .expectedVal = 15},
    (IntegerTest){.input = "var a = 5; a; a; a;", .expectedVal = 5}
  };

  for (const auto& test : tests) {
    Object* evaluated = TestEval_(test.input);
    if (!TestIntegerObject_(evaluated, test.expectedVal)) {
      return;
    }
  }

  evaluator_.FinalCleanup();
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
    },
    (ErrorTest){
      .input = "\"Hello\" - \"Hello\"",
      .expectedVal = "unknown operator: STRING - STRING"
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

  evaluator_.FinalCleanup();

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

  evaluator_.FinalCleanup();
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

  evaluator_.FinalCleanup();
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

  evaluator_.FinalCleanup();
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

  evaluator_.FinalCleanup();
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

  evaluator_.FinalCleanup();
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
  std::unordered_map<std::string, BuiltIn*> builtInFuncs = GetBuiltIns();

  Evaluator evaluator(gCollector, TRUE, FALSE, NULL_T, builtInFuncs);
  EvaluatorTest eTest(evaluator);

  eTest.Run();

  return 0;
}
