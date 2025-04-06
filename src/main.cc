#include <lexer.h>
#include <parser.h>
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
    std::cin >> input;

    auto l = std::make_shared<Lexer>(input);
    auto p = std::make_shared<Parser>(l);
    std::unique_ptr<Program> program = p->ParseProgram();

    if (p->GetErrors().size() > 0) {
      PrintParserErrors(p->GetErrors());
    }
    

    for (const auto& stmt : program->GetStatements()) {
      std::cout << stmt->String();
      std::cout << std::endl;
    }
  }


  return 0;
}