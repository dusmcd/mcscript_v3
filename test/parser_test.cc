#include <parser.h>
#include <iostream>

struct Test {
    std::string literal;
  };


bool TestVarStatement(std::shared_ptr<VarStatement> vs, Test test) {
  if (vs->TokenLiteral().compare("var") != 0) {
    std::cout << "not a var statemnt. expected var, got \n" << vs->TokenLiteral();
    return false;
  }

  if (vs->GetName()->GetValue().compare(test.literal) != 0) {
    std::cout << "wrong identifier. expected " << test.literal
          << ", got " << vs->GetName()->GetValue() << "\n";
    return false;
  }

  if (vs->GetName()->GetToken()->GetLiteral().compare(test.literal) != 0) {
    std::cout << "wrong identifier. expected " << test.literal
      << ", got " << vs->GetName()->GetToken()->GetLiteral();
    return false;
  }

  return true;
}

bool CheckParserErrors(std::shared_ptr<Parser> p) {
  int num_errors = p->GetErrors().size();
  if (num_errors == 0)
    return false;
  
  printf("parser has %d errors\n", num_errors);
  std::cout << "parser has " << num_errors << " errors\n";

  for (std::string err : p->GetErrors()) {
    std::cout << "parser error: " << err << "\n";
  }

  return true;

}

void TestVarStatements() {
  std::string input = "var x = 10;"
                      "var num = 5;";
  
  std::shared_ptr<Lexer> l = std::make_shared<Lexer>(input);
  std::shared_ptr<Parser> p = std::make_shared<Parser>(l);

  std::unique_ptr<Program> program = p->ParseProgram();
  if (CheckParserErrors(p)) {
    return;
  }

  if (program == nullptr) {
    std::cout << "program = nullptr\n";
    return;
  }

  if (program->GetStatements().size() != 2) {
    std::cout << "program should have 2 statements\n";
    return;
  }

  
  std::vector<Test> tests = {
    {.literal = "x"},
    {.literal = "num"},
  };

  std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
  for (size_t i = 0; i < stmts.size(); i++) {
    auto vs = std::dynamic_pointer_cast<VarStatement>(stmts[i]);
    if (vs == nullptr) {
      std::cout << "statement not a var statement\n";
      return;
    }
    if (!TestVarStatement(vs, tests[i])) {
      return;
    }
  }

  std::cout << "TestVarStatements() passed\n";
}

void TestReturnStatements() {
  std::string input = "return 5;"
                      "return 8904832;";

  auto l = std::make_shared<Lexer>(input);
  auto p = std::make_shared<Parser>(l);
  std::unique_ptr<Program> program = p->ParseProgram();
  if (program == nullptr) {
    std::cout << "program = nullptr\n";
    return;
  } 

  if (CheckParserErrors(p)) {
    return;
  }

  if (program->GetStatements().size() != 2) {
    std::cout << "program.statements_ does not contain 2 arguments. got " 
          << program->GetStatements().size() << "\n";
    return;
  }

  for (auto& stmt : program->GetStatements()) {
    auto rs = std::dynamic_pointer_cast<ReturnStatement>(stmt);
    if (rs == nullptr) {
      std::cout << "statement not a return statement\n";
      return;
    }

    if (rs->TokenLiteral().compare("return") != 0) {
      std::cout << "wront literal. expected 'return', got " << rs->TokenLiteral() << "\n";
      return;
    }
  }

  std::cout << "TestReturnStatements() passed\n";
}


int main() {
  TestVarStatements();
  TestReturnStatements();

  return 0;
}