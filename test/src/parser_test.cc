#include <parser.h>
#include <ast.h>
#include <iostream>
#include <parser_test.h>
#include <type_traits>

/*
==========================================
PUBLIC METHODS
==========================================
*/

void ParserTest::Run() {
  // call all main test methods
    TestInfixExpressions_();
    TestPrefixExpressions_();
    TestIntegerLiterals_();
    TestIdentityExpressions_();
    TestString_();
    TestVarStatements_();
    TestReturnStatements_();
    TestOperatorPrecedence_();
}

/*
==========================================
PRIVATE METHODS
==========================================
*/

/*
  helper methods
*/

bool ParserTest::TestBooleanExpression_(std::shared_ptr<Expression> exp, bool value) {
  auto boolExp = std::dynamic_pointer_cast<BooleanExpression>(exp);
  if (boolExp == nullptr) {
    std::cerr << "boolExp not BooleanExpression\n";
    return false;
  }

  if (boolExp->GetValue() != value) {
    std::cerr << "boolExp->GetValue() not " << value
          << ", got=" << boolExp->GetValue() << "\n";
    return false;
  }

  return true;
}

bool ParserTest::CheckParserErrors_(std::shared_ptr<Parser> p) {
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

bool ParserTest::TestIntegerLiteral_(std::shared_ptr<Expression> exp, long value) {
  auto il = std::dynamic_pointer_cast<IntegerLiteral>(exp);
  if (exp == nullptr) {
    std::cerr << "exp not IntegerLiteral\n";
    return false;
  }

  if (il->GetValue() != value) {
    std::cerr << "Right expression wrong value. expected: " << value
        << ", got: " << il->GetValue() << "\n";
    
    return false;
  }

  return true;
}

bool ParserTest::TestVarStatement_(std::shared_ptr<VarStatement> vs, Test test) {
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

template <typename T>
bool ParserTest::TestLiteralExpression_(std::shared_ptr<Expression> exp, T value) {
  if constexpr (std::is_same_v<T, long>) {
    return TestIntegerLiteral_(exp, value);
  } else if constexpr (std::is_same_v<T, std::string>) {
    return TestIdentityExpression_(exp, value); 
  } else if constexpr (std::is_same_v<T, bool>) {
    return TestBooleanExpression_(exp, value);
  }

  std::cerr << "value of type T not supported\n";
  return false;
}

bool ParserTest::TestIdentityExpression_(std::shared_ptr<Expression> exp, std::string value) {
  auto i = std::dynamic_pointer_cast<Identifier>(exp);
  if (i == nullptr) {
    std::cerr << "exp not an Identifier\n";
    return false;
  }

  if (i->GetValue().compare(value) != 0) {
    std::cerr << "i value wrong. expected: " << value 
        << ", got: " << i->GetValue() << "\n";
    return false;
  }

  return true;
}

template <typename T>
bool ParserTest::TestInfixExpression_(
      std::shared_ptr<Expression> exp,
      T left,
      std::string op,
      T right
    ) {
  auto infix = std::dynamic_pointer_cast<InfixExpression>(exp);
  if (infix == nullptr) {
    std::cerr << "infix is not of type InfixExpression\n";
    return false;
  }

  if (!TestLiteralExpression_(infix->GetLeft(), left)) {
    return false;
  }

  if (infix->GetOp().compare(op) != 0) {
    std::cerr << "infix->GetOp() does not equal " << op
        << ", got=" << infix->GetOp() << "\n";
    return false;
  }

  if (!TestLiteralExpression_(infix->GetRight(), right)) {
    return false;
  }

  return true;
}


/*
  end helper methods
*/


/*
  main test methods
*/
void ParserTest::TestInfixExpressions_() {
  std::vector<InfixTest<long>> tests = {
    (InfixTest<long>){.input = "5 + 5;", .left = 5, .op = "+", .right = 5},
    (InfixTest<long>){.input = "5 - 5;", .left = 5, .op = "-", .right = 5},
    (InfixTest<long>){.input = "5 * 5;", .left = 5, .op = "*", .right = 5},
    (InfixTest<long>){.input = "5 / 5;", .left = 5, .op = "/", .right = 5},
  };

  for (auto test : tests) {
    auto l = std::make_shared<Lexer>(test.input);
    auto p = std::make_shared<Parser>(l);
    std::unique_ptr<Program> program = p->ParseProgram();
    if (CheckParserErrors_(p)) {
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

    if (!TestInfixExpression_(es->GetExpression(), test.left, test.op, test.right)) {
      return;
    }
  }
  std::vector<InfixTest<bool>> bTests = {
    InfixTest<bool>{.input = "true == true", .left = true, .op = "==", .right = true},
    InfixTest<bool>{.input = "false != true", .left = false, .op = "!=", .right = true},
    InfixTest<bool>{.input = "false == false", .left = false, .op = "==", .right = false},
  };

  for (const auto& test : bTests) {
    auto l = std::make_shared<Lexer>(test.input);
    auto p = std::make_shared<Parser>(l);
    std::unique_ptr<Program> program = p->ParseProgram();
    if (CheckParserErrors_(p)) {
      return;
    }

    std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
    if (stmts.size() != 1) {
      std::cerr << "stmts.size() does not equal " << 1 << ". got=" << stmts.size()
        << "\n";
      return;
    }

    auto exp = std::dynamic_pointer_cast<ExpressionStatement>(stmts[0]);
    if (exp == nullptr) {
      std::cerr << "stmts[0] not Expression Statement\n";
      return;
    }

    if (!TestInfixExpression_(exp->GetExpression(), test.left, test.op, test.right)) {
      return;
    }
  }
  std::cout << "TestInfixExpressions() passed\n";
}

void ParserTest::TestPrefixExpressions_() {
  std::vector<PrefixTest<long>> tests = {
   (PrefixTest<long>){.value = 15, .op = "-", .input = "-15;"},
   (PrefixTest<long>){.value = 5, .op = "!", .input = "!5;"},
   (PrefixTest<long>){.value = 10, .op = "-", .input = "-10;"},
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

    if (CheckParserErrors_(p)) {
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

    if (!TestIntegerLiteral_(pe->GetRight(), test.value)) {
      return;
    }
  }

  std::cout << "TestPrefixExpressions() passed\n";
}

void ParserTest::TestIntegerLiterals_() {
  std::string input = "5;";

  auto l = std::make_shared<Lexer>(input);
  auto p = std::make_shared<Parser>(l);
  std::unique_ptr<Program> program = p->ParseProgram();
  if (program == nullptr) {
    std::cerr << "program = nullptr\n";
    return;
  }

  if (CheckParserErrors_(p)) {
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

    if (!TestIntegerLiteral_(es->GetExpression(), 5)) {
      return;
    }

    if (es->GetExpression()->TokenLiteral().compare("5") != 0) {
      std::cerr << "il->TokenLiteral() wrong. expected=5" << "got=" <<
          es->GetExpression()->TokenLiteral() << "\n";
    }

    std::cout << "TestIntegerLiteral_s() passed\n";

  }
}


void ParserTest::TestIdentityExpressions_() {
  std::string input = "foobar;";
  std::string expLiteral = "foobar";
  auto l = std::make_shared<Lexer>(input);
  auto p = std::make_shared<Parser>(l);
  std::unique_ptr<Program> program = p->ParseProgram();
  if (program == nullptr) {
    std::cerr << "program = nullptr\n";
    return;
  }

  if (CheckParserErrors_(p)) {
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

void ParserTest::TestString_() {
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



void ParserTest::TestVarStatements_() {
  std::string input = "var x = 10;"
                      "var num = 5;";
  
  std::shared_ptr<Lexer> l = std::make_shared<Lexer>(input);
  std::shared_ptr<Parser> p = std::make_shared<Parser>(l);

  std::unique_ptr<Program> program = p->ParseProgram(true);
  if (CheckParserErrors_(p)) {
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
    if (!TestVarStatement_(vs, tests[i])) {
      return;
    }
  }

  std::cout << "TestVarStatements() passed\n";
}

void ParserTest::TestReturnStatements_() {
  std::string input = "return 5;"
                      "return 8904832;";

  auto l = std::make_shared<Lexer>(input);
  auto p = std::make_shared<Parser>(l);
  std::unique_ptr<Program> program = p->ParseProgram(true);
  if (program == nullptr) {
    std::cerr << "program = nullptr\n";
    return;
  } 

  if (CheckParserErrors_(p)) {
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
      std::cerr << "wrong literal. expected 'return', got " << rs->TokenLiteral() << "\n";
      return;
    }
  }

  std::cout << "TestReturnStatements() passed\n";
}

void ParserTest::TestOperatorPrecedence_() {
  struct PTest {
    std::string input;
    std::string expected;
  };

  std::vector<PTest> tests = {
    (PTest){.input = "5 + 5", .expected = "(5 + 5)"},
    (PTest){.input = "1 + 2 + 3", .expected = "((1 + 2) + 3)"},
    (PTest){.input = "1 + 2 * 3", .expected = "(1 + (2 * 3))"},
    (PTest){.input = "10 / (2 + 3)", .expected = "(10 / (2 + 3))"},
    (PTest){.input = "10 - 4 / 2", .expected = "(10 - (4 / 2))"},
    (PTest){.input = "true == true", .expected = "(true == true)"},
    (PTest){.input = "!(x < y)", .expected = "(!(x < y))"},
    (PTest){.input = "!(false == true) != true", .expected = "((!(false == true)) != true)"}
  };

  for (const auto& test : tests) {
    auto l = std::make_shared<Lexer>(test.input);
    auto p = std::make_shared<Parser>(l);
    std::unique_ptr<Program> program = p->ParseProgram();
    if (CheckParserErrors_(p)) {
      return;
    }

    if (program->String().compare(test.expected) != 0) {
      std::cerr << "program->String() wrong. expected: " 
        << test.expected << ", got: " << program->String() << "\n";
      return;
    }
  }

  std::cout << "TestOperatorPrecedence_() passed\n";
}


int main() {
  ParserTest pTest = ParserTest();
  pTest.Run();
  return 0;

}