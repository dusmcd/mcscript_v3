#include <lexer.h>
#include <iostream>
#include <string>

int main() {
  std::cout << "McScript v3.0 Programming Language\n";
  std::cout << "Enter commands:\n";

  while (true) {
    std::cout << ">> ";
    std::string input;
    std::cin >> input;

    Lexer l = Lexer(input);

    for (std::shared_ptr<Token> tok = l.NextToken(); tok->GetType() != TokenType::EOI; tok = l.NextToken()) {
      std::cout << "{type: " << static_cast<int>(tok->GetType()) << ", literal: " << tok->GetLiteral() << "}\n";
    }

  }


  return 0;
}