#include <parser.h>
#include <ast.h>
#include <iostream>

struct Test {
    std::string literal;
  };

void TestIdentityExpressions() {
  std::string input = "foobar;";
  std::string expLiteral = "foobar";
  auto l = std::make_shared<Lexer>(input);
  auto p = std::make_shared<Parser>(l);
  std::unique_ptr<Program> program = p->ParseProgram();
  if (program == nullptr) {
    std::cout << "program = nullptr\n";
    return;
  }

  std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
  if (stmts.size() != 1) {
    std::cout << "program->GetStatements().size() does not equal 1\n";
    return;
  }

  for (auto stmt : stmts) {
    auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmt);
    if (es == nullptr) {
      std::cout << "statement is not an expression statement\n";
      return;
    }

    auto i = std::dynamic_pointer_cast<Identifier>(es->GetExpression());
    if (i == nullptr) {
      std::cout << "expression is not an identity expression\n";
      return;
    }

    if (i->GetValue().compare(expLiteral) != 0) {
      std::cout << "identifier value wrong. expected: " << expLiteral
            << ", got: " << i->GetValue() << "\n";
      return;
    }
  }

  std::cout << "TestIdentityExpressions() passed\n";
}

void TestString() {
  auto vs = std::make_shared<VarStatement>(std::make_shared<Token>(TokenType::VAR, "var"));
  
  vs->SetName(std::make_shared<Identifier>("myVar", std::make_shared<Token>(TokenType::IDENT, "myVar")));
  vs->SetValue(std::make_shared<Identifier>("anotherVar", std::make_shared<Token>(TokenType::IDENT, "anotherVar")));
 
  std::vector<std::shared_ptr<Statement>> stmts = {
    vs
  };

  for (auto stmt : stmts) {
    const std::string test = "var myVar = anotherVar;";
    std::string actual = stmt->String();

    if (test.compare(actual) != 0) {
      std::cout << "TestString() failed\n";
      return;
    }
  }
  std::cout << "TestString() passed\n";
}


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

  std::unique_ptr<Program> program = p->ParseProgram(true);
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
  std::unique_ptr<Program> program = p->ParseProgram(true);
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
  TestString();
  TestIdentityExpressions();

  return 0;
}