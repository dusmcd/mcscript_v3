#include <lexer.h>
#include <parser.h>
#include <evaluator.h>
#include <iostream>
#include <string>

void PrintParserErrors(std::vector<std::string> errs) {
  for (const std::string& err : errs) {
    std::cerr << err << "\n";
  }
}

std::shared_ptr<Evaluator> NewEval() {
  GCollector& gCollector = GCollector::getGCollector();
  Boolean* TRUE = new Boolean(true);
  Boolean* FALSE = new Boolean(false);
  Null* NULL_T = new Null();
  auto evaluator = std::make_shared<Evaluator>(gCollector, TRUE, FALSE, NULL_T);

  return evaluator;
}

int main() {
  Environment& env = Environment::NewEnvironment();
  std::shared_ptr<Evaluator> evaluator = NewEval();

  std::cout << "McScript v3.0 Programming Language\n";
  std::cout << "Enter commands:\n";

  while (true) {
    std::cout << ">> ";
    std::string input;
    std::getline(std::cin, input);

    auto l = std::make_shared<Lexer>(input);
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();

    if (p->GetErrors().size() > 0) {
      PrintParserErrors(p->GetErrors());
    }
    

    Object* obj = evaluator->Eval(program, env);
    evaluator->TrackObject(obj);
    if (obj != nullptr) {
      std::cout << obj->Inspect();
      std::cout << std::endl;
    }

    
  }

  
  evaluator->CollectGarbage();
  return 0;
}