#include <parser.h>
#include <ast.h>
#include <iostream>

struct Test {
    std::string literal;
  };

struct PrefixTest {
  long value;
  std::string op;
  std::string input;
};

bool CheckParserErrors(std::shared_ptr<Parser> p) {
  int num_errors = p->GetErrors().size();
  if (num_errors == 0)
    return false;
  
  printf("parser has %d errors\n", num_errors);
  std::cerr << "parser has " << num_errors << " errors\n";

  for (std::string err : p->GetErrors()) {
    std::cerr << "parser error: " << err << "\n";
  }

  return true;

}

bool TestIntegerLiteral(std::shared_ptr<IntegerLiteral> il, int value) {
  if (il->GetValue() != value) {
    std::cerr << "Right expression wrong value. expected: " << value
        << ", got: " << il->GetValue() << "\n";
    
    return false;
  }

  return true;
}


void TestInfixExpressions() {
  struct InfixTest {
    std::string input;
    long left;
    std::string op;
    long right;
  };
  std::vector<InfixTest> tests = {
    (InfixTest){.input = "5 + 5;", .left = 5, .op = "+", .right = 5},
    (InfixTest){.input = "5 - 5;", .left = 5, .op = "-", .right = 5},
    (InfixTest){.input = "5 * 5;", .left = 5, .op = "*", .right = 5},
    (InfixTest){.input = "5 / 5;", .left = 5, .op = "/", .right = 5},
  };

  for (auto test : tests) {
    auto l = std::make_shared<Lexer>(test.input);
    auto p = std::make_shared<Parser>(l);
    std::unique_ptr<Program> program = p->ParseProgram();
    if (CheckParserErrors(p)) {
      return;
    }

    std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
    if (stmts.size() != 1) {
      std::cerr << "stmt.size() does not equal 1. got=" << stmts.size() << "\n";
      return;
    }

    auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmts[0]);
    if (es == nullptr) {
      std::cerr << "stmt is not an ExpressionStatement\n";
      return;
    }

    auto ie = std::dynamic_pointer_cast<InfixExpression>(es->GetExpression());
    if (ie == nullptr) {
      std::cerr << "expression is not an InfixExpression\n";
      return;
    }

    auto left = std::dynamic_pointer_cast<IntegerLiteral>(ie->GetLeft());
    if (left == nullptr) {
      std::cerr << "left expression is not an IntegerLiteral\n";
      return;
    } 

    auto right = std::dynamic_pointer_cast<IntegerLiteral>(ie->GetRight());
    if (right == nullptr) {
      std::cerr << "right expression is not an IntegerLiteral\n";
      return;
    }

    if (test.op.compare(ie->GetOp()) != 0) {
      std::cerr << "wrong operator. expected: " << test.op 
            << ", got: " << ie->GetOp() << "\n";
      return;
    }

    if (!TestIntegerLiteral(left, test.left)) {
      return;
    }

    if (!TestIntegerLiteral(right, test.right)) {
      return;
    }

    std::cout << "TestInfixExpressions() passed\n";

  }

  }




void TestPrefixExpressions() {
  std::vector<PrefixTest> tests = {
   (PrefixTest){.value = 15, .op = "-", .input = "-15;"},
   (PrefixTest){.value = 5, .op = "!", .input = "!5;"},
   (PrefixTest){.value = 10, .op = "-", .input = "-10;"},
  };

  for (size_t i = 0; i < tests.size(); i++) {
    PrefixTest test = tests[i];
    auto l = std::make_shared<Lexer>(test.input);
    auto p = std::make_shared<Parser>(l);
    std::unique_ptr<Program> program = p->ParseProgram();
    if (program == nullptr) {
      std::cerr << "program = nullptr\n";
      return;
    }

    if (CheckParserErrors(p)) {
      return;
    }

    std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
    if (stmts.size() != 1) {
      std::cerr << "program->GetStatements().size() does not equal 1. got="
              << stmts.size() << "\n";
      return;
    }

    auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmts[0]);
    if (es == nullptr) {
      std::cerr << "stmts[0] not an ExpressionStatement\n";
      return;
    }

    auto pe = std::dynamic_pointer_cast<PrefixExpression>(es->GetExpression());
    if (pe == nullptr) {
      std::cerr << "es->GetExpression() not a PrefixExpression\n";
      return;
    }

    if (pe->TokenLiteral().compare(test.op)) {
      std::cerr << "wrong operator. expected: " << test.op << ", got = "
          << pe->TokenLiteral() << "\n";
      return;
    }

    auto il = std::dynamic_pointer_cast<IntegerLiteral>(pe->GetRight());
    if (il == nullptr) {
      std::cerr << "pe->GetRight() not an IntegerLiteral\n";
      return;
    }

    if (!TestIntegerLiteral(il, test.value)) {
      return;
    }
  }

  std::cout << "TestPrefixExpressions() passed\n";
}

void TestIntegerLiterals() {
  std::string input = "5;";

  auto l = std::make_shared<Lexer>(input);
  auto p = std::make_shared<Parser>(l);
  std::unique_ptr<Program> program = p->ParseProgram();
  if (program == nullptr) {
    std::cerr << "program = nullptr\n";
    return;
  }

  if (CheckParserErrors(p)) {
    return;
  }

  std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
  if (stmts.size() != 1) {
    std::cerr << "program->GetStatements() does not have 1 statement. got=" <<
              stmts.size() << "\n";
    return;
  }

  for (auto stmt : stmts) {
    auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmt);
    if (es == nullptr) {
      std::cerr << "statement not an expression\n";
      return;
    }

    auto il = std::dynamic_pointer_cast<IntegerLiteral>(es->GetExpression());
    if (il == nullptr) {
      std::cerr << "expression not an integer literal\n";
      return;
    }

    if (il->GetValue() != 5) {
      std::cerr << "il->GetValue() wrong. expected=" << 5 << " got=" << 
          il->GetValue() << "\n";
      return;
    }

    if (il->TokenLiteral().compare("5") != 0) {
      std::cerr << "il->TokenLiteral() wrong. expected=5" << "got=" <<
          il->TokenLiteral() << "\n";
    }

    std::cout << "TestIntegerLiterals() passed\n";

  }
}


void TestIdentityExpressions() {
  std::string input = "foobar;";
  std::string expLiteral = "foobar";
  auto l = std::make_shared<Lexer>(input);
  auto p = std::make_shared<Parser>(l);
  std::unique_ptr<Program> program = p->ParseProgram();
  if (program == nullptr) {
    std::cerr << "program = nullptr\n";
    return;
  }

  if (CheckParserErrors(p)) {
    return;
  }

  std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
  if (stmts.size() != 1) {
    std::cerr << "program->GetStatements().size() does not equal 1\n";
    return;
  }

  for (auto stmt : stmts) {
    auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmt);
    if (es == nullptr) {
      std::cerr << "statement is not an expression statement\n";
      return;
    }

    auto i = std::dynamic_pointer_cast<Identifier>(es->GetExpression());
    if (i == nullptr) {
      std::cerr << "expression is not an identity expression\n";
      return;
    }

    if (i->GetValue().compare(expLiteral) != 0) {
      std::cerr << "identifier value wrong. expected: " << expLiteral
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
      std::cerr << "TestString() failed\n";
      return;
    }
  }
  std::cerr << "TestString() passed\n";
}


bool TestVarStatement(std::shared_ptr<VarStatement> vs, Test test) {
  if (vs->TokenLiteral().compare("var") != 0) {
    std::cerr << "not a var statemnt. expected var, got \n" << vs->TokenLiteral();
    return false;
  }

  if (vs->GetName()->GetValue().compare(test.literal) != 0) {
    std::cerr << "wrong identifier. expected " << test.literal
          << ", got " << vs->GetName()->GetValue() << "\n";
    return false;
  }

  if (vs->GetName()->GetToken()->GetLiteral().compare(test.literal) != 0) {
    std::cerr << "wrong identifier. expected " << test.literal
      << ", got " << vs->GetName()->GetToken()->GetLiteral();
    return false;
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
    std::cerr << "program = nullptr\n";
    return;
  }

  if (program->GetStatements().size() != 2) {
    std::cerr << "program should have 2 statements\n";
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
      std::cerr << "statement not a var statement\n";
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
    std::cerr << "program = nullptr\n";
    return;
  } 

  if (CheckParserErrors(p)) {
    return;
  }

  if (program->GetStatements().size() != 2) {
    std::cerr << "program.statements_ does not contain 2 arguments. got " 
          << program->GetStatements().size() << "\n";
    return;
  }

  for (auto& stmt : program->GetStatements()) {
    auto rs = std::dynamic_pointer_cast<ReturnStatement>(stmt);
    if (rs == nullptr) {
      std::cerr << "statement not a return statement\n";
      return;
    }

    if (rs->TokenLiteral().compare("return") != 0) {
      std::cerr << "wront literal. expected 'return', got " << rs->TokenLiteral() << "\n";
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
  TestIntegerLiterals();
  TestPrefixExpressions();
  TestInfixExpressions();

  return 0;
}