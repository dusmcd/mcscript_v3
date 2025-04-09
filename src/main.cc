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

int main() {
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
    
    std::shared_ptr<Object> obj = Eval(program);
    if (obj != nullptr) {
      std::cout << obj->Inspect();
    }
    std::cout << std::endl;
  }


  return 0;
}