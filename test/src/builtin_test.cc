#include <builtin_test.h>
#include <object.h>
#include <lexer.h>
#include <parser.h>
#include <iostream>

void BuiltInTest::Run() {
  TestLen_();
  TestPush_();
}

/*
  main test methods
*/


void BuiltInTest::TestPush_() {
  std::string input = "var arr = [1, 2, 3]; push(arr, 5); return arr[3];";

  Object* obj = TestEval_(input);
  if (!TestIntegerObject_(obj, 5)) {
    return;
  }

  evaluator_.FinalCleanup();
  std::cout << "TestPush_() passed\n";

}

Object* BuiltInTest::TestEval_(std::string input) {
  auto l = std::make_shared<Lexer>(input.c_str());
  auto p = std::make_shared<Parser>(l);
  std::shared_ptr<Program> program = p->ParseProgram();

  auto env = std::make_shared<Environment<Object*>>();

  return evaluator_.Eval(program, env);
}

void BuiltInTest::TestLen_() {
  std::vector<IntegerTest> tests = {
    (IntegerTest){.input = "var str = \"hello\"; len(str);", .expected = 5},
    (IntegerTest){.input = "len(\"world\");", .expected = 5},
    (IntegerTest){.input = "var arr = [1, 2, 3]; len(arr);", .expected = 3},
    (IntegerTest){.input = "var arr = []; len(arr);", .expected = 0}
  };

  for (const auto& test : tests) {
    Object* obj = TestEval_(test.input);

    if (!TestIntegerObject_(obj, test.expected)) {
      return;
    }
  }

  evaluator_.FinalCleanup();
  std::cout << "TestLen_() passed\n";
}

/*
  helpers
*/

bool BuiltInTest::TestIntegerObject_(Object* obj, long expected) {
  auto integer = dynamic_cast<Integer*>(obj);
  if (integer == nullptr) {
    std::cerr << "object is not an Integer\n";
    return false;
  }

  if (integer->GetValue() != expected) {
    std::cerr << "integer wrong value. expected: " 
        << expected << ", got: " << integer->GetValue() << "\n";
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

  BuiltInTest test(evaluator);

  test.Run();

  return 0;
}
