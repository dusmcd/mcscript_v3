#include <memory>
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
    TestIfExpression_();
    TestIfElseExpression_();
    TestFunctionLiteral_();
    TestCallExpressions_();
    TestStringLiteral_();
    TestArrayLiterals_();
    TestIndexExpressions_();
    TestAssignExpressions_();
    TestForStatements_();

}

/*
==========================================
PRIVATE METHODS
==========================================
*/

/*
  helper methods
*/

bool ParserTest::TestBlockStatement_(std::shared_ptr<BlockStatement> block, size_t size, expressionVal value) {
  std::string val = std::get<std::string>(value);
  std::vector<std::shared_ptr<Statement>> bStmts = block->GetStatements();
  if (bStmts.size() != size) {
    std::cerr << "bStmts.size() does not equal " << 1 
          << " got=" << bStmts.size() << "\n";
    return false;
  }

  auto es = std::dynamic_pointer_cast<ExpressionStatement>(bStmts[0]);
  if (es == nullptr) {
    std::cerr << "bStmts[0] is not ExpressionStatement\n";
    return false;
  }

  if (!TestLiteralExpression_(es->GetExpression(), val)) {
    return false;
  }
  return true;

}

bool ParserTest::TestBooleanExpression_(std::shared_ptr<Expression> exp, expressionVal value) {
  bool val = std::get<bool>(value);
  auto boolExp = std::dynamic_pointer_cast<BooleanExpression>(exp);
  if (boolExp == nullptr) {
    std::cerr << "boolExp not BooleanExpression\n";
    return false;
  }

  if (boolExp->GetValue() != val) {
    std::cerr << "boolExp->GetValue() not " << val
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

bool ParserTest::TestIntegerLiteral_(std::shared_ptr<Expression> exp, expressionVal value) {
  long val = std::get<long>(value);
  auto il = std::dynamic_pointer_cast<IntegerLiteral>(exp);
  if (exp == nullptr) {
    std::cerr << "exp not IntegerLiteral\n";
    return false;
  }

  if (il->GetValue() != val) {
    std::cerr << "Right expression wrong value. expected: " << val
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

bool ParserTest::TestLiteralExpression_(std::shared_ptr<Expression> exp, expressionVal value) {
  if (std::holds_alternative<long>(value)) {
    return TestIntegerLiteral_(exp, value);
  } else if (std::holds_alternative<std::string>(value)) {
    return TestIdentityExpression_(exp, value); 
  } else if (std::holds_alternative<bool>(value)) {
    return TestBooleanExpression_(exp, value);
  }

  std::cerr << "value of type T not supported\n";
  return false;
}

bool ParserTest::TestIdentityExpression_(std::shared_ptr<Expression> exp, expressionVal value) {
  std::string val = std::get<std::string>(value);
  auto i = std::dynamic_pointer_cast<Identifier>(exp);
  if (i == nullptr) {
    std::cerr << "exp not an Identifier\n";
    return false;
  }

  if (i->GetValue().compare(val) != 0) {
    std::cerr << "i value wrong. expected: " << val
        << ", got: " << i->GetValue() << "\n";
    return false;
  }

  return true;
}

bool ParserTest::TestInfixExpression_(
      std::shared_ptr<Expression> exp,
      expressionVal left,
      std::string op,
      expressionVal right
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

void ParserTest::TestForStatements_() {
  std::string input = "for (var i = 0; i < 10; i = i + 1){ i; }";

  auto l = std::make_shared<Lexer>(input.c_str());
  auto p = std::make_shared<Parser>(l);
  std::shared_ptr<Program> program = p->ParseProgram();
  if (CheckParserErrors_(p)) {
    return;
  }

  std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();

  if (stmts.size() != 1) {
    std::cerr << "program does not contain 1 statement. got=" << 
      stmts.size() << "\n";
    return;
  }

  auto forStmt = std::dynamic_pointer_cast<ForStatement>(stmts[0]);
  if (forStmt == nullptr) {
    std::cerr << "stmts[0] is not a ForStatement\n";
    return;
  }

  Test test = {.literal = "i"};
  if (!TestVarStatement_(forStmt->GetVarStmt(), test)) {
    return;
  }

  if (!TestInfixExpression_(forStmt->GetCondition(), std::string("i"), std::string("<"), 10)) {
    return;
  }

  auto assign = std::dynamic_pointer_cast<AssignExpression>(forStmt->GetAfterAction());
  if (assign == nullptr) {
    std::cerr << "after action not AssignExpression\n";
    return;
  }

  if (!TestInfixExpression_(assign->GetNewVal(), std::string("i"), std::string("+"), 1)) {
    return;
  }

  if (!TestBlockStatement_(forStmt->GetBlock(),  1, std::string("i"))) {
    return;
  }

  std::cout << "TestForStatements_() passed\n";


}

void ParserTest::TestAssignExpressions_() {
  std::vector<std::string> tests = {"i = 10;", "i = i + 1"};

  for (size_t i = 0; i < tests.size(); i++) {
    std::string test = tests[i];
    auto l = std::make_shared<Lexer>(test.c_str());
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();

    if (CheckParserErrors_(p)) {
      return;
    }

    std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();

    if (stmts.size() != 1) {
      std::cerr << "stmts.size() does not equal 1. got="
        << stmts.size() << "\n";
      return;
    }

    auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmts[0]);
    if (es == nullptr) {
      std::cerr << "stmts[0] is not ExpressionStatement\n";
      return;
    }

    auto assign = std::dynamic_pointer_cast<AssignExpression>(es->GetExpression());
    if (assign == nullptr) {
      std::cerr << "es->GetExpression() is not AssignExpression\n";
      return;
    }

    if (!TestLiteralExpression_(assign->GetIdent(), std::string("i"))) {
      return;
    }

    if (i == 0) {
      if (!TestLiteralExpression_(assign->GetNewVal(), 10)) {
        return;
      }
    } else {
      if (!TestInfixExpression_(assign->GetNewVal(), std::string("i"), std::string("+"), 1)) {
        return;
      }
    }
  }
  std::cout << "TestAssignExpressions_() passed\n";

}

void ParserTest::TestIndexExpressions_() {
  std::vector<IndexTest> tests = {
    (IndexTest){.input = "arr[0]", .expectedIndex = 0, .expectedIdent = "arr"},
    (IndexTest){.input = "arr[i]", .expectedIndex = std::string("i"), .expectedIdent = "arr"}
  };


  for (const auto& test : tests) {
    auto l = std::make_shared<Lexer>(test.input.c_str());
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();
    if (CheckParserErrors_(p)) {
      return;
    }

    std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();

    if (stmts.size() != 1) {
      std::cerr << "stmts does not have 1 statement. got=" << stmts.size()
        << "\n";
      return;
    }

    auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmts[0]);
    if (es == nullptr) {
      std::cerr << "stmts[0] is not ExpressionStatement\n";
      return;
    }

    auto iExp = std::dynamic_pointer_cast<IndexExpression>(es->GetExpression());
    if (iExp == nullptr) {
      std::cerr << "es->GetExpression() not IndexExpression\n";
      return;
    }

    if (!TestLiteralExpression_(iExp->GetExp(), test.expectedIdent)) {
      return;
    }

    if (!TestLiteralExpression_(iExp->GetIdx(), test.expectedIndex)) {
      return;
    }
  }

  std::cout << "TestIndexExpressions_() passed\n";
}


void ParserTest::TestArrayLiterals_() {
  std::string input = "[1, 2, 3 * 5];";

  auto l = std::make_shared<Lexer>(input.c_str());
  auto p = std::make_shared<Parser>(l);
  std::shared_ptr<Program> program = p->ParseProgram();
  if (CheckParserErrors_(p)) {
    return;
  }

  std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
  if (stmts.size() != 1) {
    std::cerr << "program does not contain 1 statement. got=" << stmts.size() << "\n";
    return;
  }

  auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmts[0]);
  if (es == nullptr) {
    std::cerr << "stmts[0] is not ExpressionStatement\n";
    return;
  }

  auto arr = std::dynamic_pointer_cast<ArrayLiteral>(es->GetExpression());
  if (arr == nullptr) {
    std::cerr << "es->GetExpression() not ArrayLiteral\n";
    return;
  }
  
  std::vector<std::shared_ptr<Expression>> exps = arr->GetExps();

  if (!TestLiteralExpression_(exps[0], 1) || !TestLiteralExpression_(exps[1], 2)) {
    return;
  }

  if (!TestInfixExpression_(exps[2], 3, std::string("*"), 5)) {
    return;
  }

  std::cout << "TestArrayLiterals_() passed\n";
}


void ParserTest::TestStringLiteral_() {
  std::string input = "\"hello world\"";

  auto l = std::make_shared<Lexer>(input.c_str());
  auto p = std::make_shared<Parser>(l);
  std::shared_ptr<Program> program = p->ParseProgram();
  if (CheckParserErrors_(p)) {
    return;
  }

  std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
  auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmts[0]);
  if (es == nullptr) {
    std::cerr << "stmts[0] not ExpressionStatement\n";
    return;
  }

  auto str = std::dynamic_pointer_cast<StringLiteral>(es->GetExpression());
  if (str == nullptr) {
    std::cerr << "es->GetExpression() not StringLiteral\n";
    return;
  }

  if (str->TokenLiteral().compare("hello world") != 0) {
    std::cerr << "wrong string value. expected: \"hello world\", got: " 
        << str->TokenLiteral() << "\n";
    return;
  }

  std::cout << "TestStringLiteral_() passed\n";
}

void ParserTest::TestCallExpressions_() {
  std::vector<CallTest> tests = {
    (CallTest){.input = "add(1, 2);", .expectedParams = std::vector<expressionVal>{1, 2}},
    (CallTest){.input = "add(true);", .expectedParams = std::vector<expressionVal>{true}},
    (CallTest){.input = "add();", .expectedParams = std::vector<expressionVal>{}}
  };

  for (const auto& test : tests) {
    auto l = std::make_shared<Lexer>(test.input.c_str());
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();
    if (CheckParserErrors_(p)) {
      return;
    }

    std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();

    if (stmts.size() != 1) {
      std::cerr << "program does not contain " << 1 << " statement. got="
          << stmts.size() << "\n";
      return;
    }

    auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmts[0]);
    if (es == nullptr) {
      std::cerr << "stmts[0] is not an ExpressionStatement\n";
      return;
    }

    auto call = std::dynamic_pointer_cast<CallExpression>(es->GetExpression());
    if (call == nullptr) {
      std::cerr << "es->GetExpression() is not a CallExpression\n";
      return;
    }

    std::vector<std::shared_ptr<Expression>> args = call->GetArgs();

    if (args.size() != test.expectedParams.size()) {
      std::cerr << "wrong number of arguments. expected: "
          << test.expectedParams.size() << ", got: " << args.size()
          << "\n";
      return;
    }

    for (size_t i = 0; i < args.size(); i++) {
      if (!TestLiteralExpression_(args[i], test.expectedParams[i])) {
        return;
      }
    }
  }

  std::cout << "TestCallExpressions_() passed\n";
}

void ParserTest::TestFunctionLiteral_() {
  std::vector<FunctionTest> tests = {
    (FunctionTest){.input = "function(a) { a; }", .expectedParams = std::vector<std::string>{"a"}, .body = std::string("a")},
    (FunctionTest){.input = "function() { x; }", .expectedParams = std::vector<std::string>{}, .body = std::string("x")},
    (FunctionTest){.input = "function(a, b) { b; }", .expectedParams = std::vector<std::string>{"a", "b"}, .body = std::string("b")},
  };

  for (const auto& test : tests) {
    auto l = std::make_shared<Lexer>(test.input.c_str());
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();
    if (CheckParserErrors_(p)) {
      return;
    }

    std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
    if (stmts.size() != 1) {
      std::cerr << "stmts.size() does not equal " << 1 << ", got=" << stmts.size() << "\n";
      return;
    }

    auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmts[0]);
    if (es == nullptr) {
      std::cerr << "stmts[0] not ExpressionStatement\n";
      return;
    }

    auto function = std::dynamic_pointer_cast<FunctionLiteral>(es->GetExpression());
    if (function == nullptr) {
      std::cerr << "es->GetExpression() not a FunctionLiteral\n";
      return;
    }

    if (test.expectedParams.size() != function->GetParameters().size()) {
      std::cerr << "wrong number of parameters. expected: " <<
          test.expectedParams.size() << ", got: " << function->GetParameters().size()
          << "\n";
      return;
    }

    for (size_t i = 0; i < function->GetParameters().size(); i++) {
      if (!TestIdentityExpression_(function->GetParameters()[i], test.expectedParams[i])){
        return;
      }
    }

    auto body = std::dynamic_pointer_cast<BlockStatement>(function->GetBody());
    if (body == nullptr) {
      std::cerr << "function->GetBody() not a BlockStatement\n";
      return;
    }

    if (!TestBlockStatement_(body, 1, test.body)) {
      return;
    }
  }

  std::cout << "TestFunctionLiteral_() passed\n";
}


void ParserTest::TestIfElseExpression_() {
  std::string input = "if (x > y) { x } else { y }";

  auto l = std::make_shared<Lexer>(input.c_str());
  auto p = std::make_shared<Parser>(l);
  std::shared_ptr<Program> program = p->ParseProgram();
  if (CheckParserErrors_(p)) {
    return;
  }

  std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
  if (stmts.size() != 1) {
    std::cerr << "stmts.size() does not equal " << 1 << " got=" << stmts.size() << "\n";
    return;
  }

  auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmts[0]);
  if (es == nullptr) {
    std::cerr << "stmts[0] not ExpressionStatement \n";
    return;
  }

  auto ifExp = std::dynamic_pointer_cast<IfExpression>(es->GetExpression());
  if (ifExp == nullptr) {
    std::cerr << "es->GetExpression() not IfExpression\n";
    return;
  }

  if (!TestInfixExpression_(ifExp->GetCondition(), std::string("x"), ">", std::string("y"))) {
    return;
  }

  std::shared_ptr<BlockStatement> block = ifExp->GetConsequence();
  if (!TestBlockStatement_(block, 1, std::string("x"))) {
    return;
  }

  std::shared_ptr<BlockStatement> alt = ifExp->GetAlternative();
  if (!TestBlockStatement_(alt, 1, std::string("y"))) {
    return;
  }

  std::cout << "TestIfElseExpression_() passed\n";

}

void ParserTest::TestIfExpression_() {
  std::string input = "if (x > y) { x }";

  auto l = std::make_shared<Lexer>(input.c_str());
  auto p = std::make_shared<Parser>(l);
  std::shared_ptr<Program> program = p->ParseProgram();

  if (CheckParserErrors_(p)) {
    return;
  }

  std::vector<std::shared_ptr<Statement>> stmts = program->GetStatements();
  if (stmts.size() != 1) {
    std::cerr << "stmts.size() does not equal " << 1 << ", got=" 
            << stmts.size() << "\n";
    return;
  }

  auto es = std::dynamic_pointer_cast<ExpressionStatement>(stmts[0]);
  if (es == nullptr) {
    std::cerr << "stmts[0] is not ExpressionStatement\n";
    return;
  }

  auto ifExp = std::dynamic_pointer_cast<IfExpression>(es->GetExpression());
  if (ifExp == nullptr) {
    std::cerr << "es->GetExpression() is not IfExpression\n";
    return;
  }

  if (!TestInfixExpression_(ifExp->GetCondition(), std::string("x"), ">", std::string("y"))) {
    return;
  }

  std::shared_ptr<BlockStatement> block = ifExp->GetConsequence();
  if (!TestBlockStatement_(block, 1, std::string("x"))) {
    return;
  }

  if (ifExp->GetAlternative() != nullptr) {
    std::cerr << "ifExp->GetAlternative() should be null. no else statement\n";
    return;
  }

  std::cout << "TestIfExpression_() passed\n";
  
}

void ParserTest::TestInfixExpressions_() {
  std::vector<InfixTest> tests = {
    (InfixTest){.input = "5 + 5;", .left = 5, .op = "+", .right = 5},
    (InfixTest){.input = "5 - 5;", .left = 5, .op = "-", .right = 5},
    (InfixTest){.input = "5 * 5;", .left = 5, .op = "*", .right = 5},
    (InfixTest){.input = "5 / 5;", .left = 5, .op = "/", .right = 5},
    (InfixTest){.input = "true == true", .left = true, .op = "==", .right = true},
    (InfixTest){.input = "false != true", .left = false, .op = "!=", .right = true},
    (InfixTest){.input = "false == false", .left = false, .op = "==", .right = false},
  };

  for (auto test : tests) {
    auto l = std::make_shared<Lexer>(test.input.c_str());
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();
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

  std::cout << "TestInfixExpressions() passed\n";
}

void ParserTest::TestPrefixExpressions_() {
  std::vector<PrefixTest> tests = {
   (PrefixTest){.value = 15, .op = "-", .input = "-15;"},
   (PrefixTest){.value = 5, .op = "!", .input = "!5;"},
   (PrefixTest){.value = 10, .op = "-", .input = "-10;"},
  };

  for (size_t i = 0; i < tests.size(); i++) {
    PrefixTest test = tests[i];
    auto l = std::make_shared<Lexer>(test.input.c_str());
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();
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

  auto l = std::make_shared<Lexer>(input.c_str());
  auto p = std::make_shared<Parser>(l);
  std::shared_ptr<Program> program = p->ParseProgram();
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
  auto l = std::make_shared<Lexer>(input.c_str());
  auto p = std::make_shared<Parser>(l);
  std::shared_ptr<Program> program = p->ParseProgram();
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
  
  std::shared_ptr<Lexer> l = std::make_shared<Lexer>(input.c_str());
  std::shared_ptr<Parser> p = std::make_shared<Parser>(l);

  std::shared_ptr<Program> program = p->ParseProgram();
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

  auto l = std::make_shared<Lexer>(input.c_str());
  auto p = std::make_shared<Parser>(l);
  std::shared_ptr<Program> program = p->ParseProgram();
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
    auto l = std::make_shared<Lexer>(test.input.c_str());
    auto p = std::make_shared<Parser>(l);
    std::shared_ptr<Program> program = p->ParseProgram();
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
