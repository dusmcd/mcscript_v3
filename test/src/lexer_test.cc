#include <lexer.h>
#include <iostream>
#include <string>
#include <vector>

void TestNextToken() {
  const char* input = "var a = 5;"
                      "var b = 10;"
                      "var add = function(a, b) {return a + b;}"
                      "5 < 10 > 11;"
                      "2 == 2;"
                      "!;"
                      "2 != 3;"
                      "if (a == b) {return true;}"
                      "else {return false;}"
                      "\"foobar\""
                      "\"foo bar\""
                      "[1, 2, 3];"
                      "for(){}";

  struct Test {
    TokenType expected_type;
    std::string expected_literal;
  };

  std::vector<Test> tests = {
      {.expected_type = TokenType::VAR, .expected_literal = "var"},
      {.expected_type = TokenType::IDENT, .expected_literal = "a"},
      {.expected_type = TokenType::ASSIGN, .expected_literal = "="},
      {.expected_type = TokenType::INT, .expected_literal = "5"},
      {.expected_type = TokenType::SEMICOLON, .expected_literal = ";"},
      {.expected_type = TokenType::VAR, .expected_literal = "var"},
      {.expected_type = TokenType::IDENT, .expected_literal = "b"},
      {.expected_type = TokenType::ASSIGN, .expected_literal = "="},
      {.expected_type = TokenType::INT, .expected_literal = "10"},
      {.expected_type = TokenType::SEMICOLON, .expected_literal = ";"},
      {.expected_type = TokenType::VAR, .expected_literal = "var"},
      {.expected_type = TokenType::IDENT, .expected_literal = "add"},
      {.expected_type = TokenType::ASSIGN, .expected_literal = "="},
      {.expected_type = TokenType::FUNCTION, .expected_literal = "function"},
      {.expected_type = TokenType::LPAREN, .expected_literal = "("},
      {.expected_type = TokenType::IDENT, .expected_literal = "a"},
      {.expected_type = TokenType::COMMA, .expected_literal = ","},
      {.expected_type = TokenType::IDENT, .expected_literal = "b"},
      {.expected_type = TokenType::RPAREN, .expected_literal = ")"},
      {.expected_type = TokenType::LBRACE, .expected_literal = "{"},
      {.expected_type = TokenType::RETURN, .expected_literal = "return"},
      {.expected_type = TokenType::IDENT, .expected_literal = "a"},
      {.expected_type = TokenType::PLUS, .expected_literal = "+"},
      {.expected_type = TokenType::IDENT, .expected_literal = "b"},
      {.expected_type = TokenType::SEMICOLON, .expected_literal = ";"},
      {.expected_type = TokenType::RBRACE, .expected_literal = "}"},
      {.expected_type = TokenType::INT, .expected_literal = "5"},
      {.expected_type = TokenType::LT, .expected_literal = "<"},
      {.expected_type = TokenType::INT, .expected_literal = "10"},
      {.expected_type = TokenType::GT, .expected_literal = ">"},
      {.expected_type = TokenType::INT, .expected_literal = "11"},
      {.expected_type = TokenType::SEMICOLON, .expected_literal = ";"},
      {.expected_type = TokenType::INT, .expected_literal = "2"},
      {.expected_type = TokenType::EQ, .expected_literal = "=="},
      {.expected_type = TokenType::INT, .expected_literal = "2"},
      {.expected_type = TokenType::SEMICOLON, .expected_literal = ";"},
      {.expected_type = TokenType::BANG, .expected_literal = "!"},
      {.expected_type = TokenType::SEMICOLON, .expected_literal = ";"},
      {.expected_type = TokenType::INT, .expected_literal = "2"},
      {.expected_type = TokenType::NOT_EQ, .expected_literal = "!="},
      {.expected_type = TokenType::INT, .expected_literal = "3"},
      {.expected_type = TokenType::SEMICOLON, .expected_literal = ";"},
      {.expected_type = TokenType::IF, .expected_literal = "if"},
      {.expected_type = TokenType::LPAREN, .expected_literal = "("},
      {.expected_type = TokenType::IDENT, .expected_literal = "a"},
      {.expected_type = TokenType::EQ, .expected_literal = "=="},
      {.expected_type = TokenType::IDENT, .expected_literal = "b"},
      {.expected_type = TokenType::RPAREN, .expected_literal = ")"},
      {.expected_type = TokenType::LBRACE, .expected_literal = "{"},
      {.expected_type = TokenType::RETURN, .expected_literal = "return"},
      {.expected_type = TokenType::TRUE, .expected_literal = "true"},
      {.expected_type = TokenType::SEMICOLON, .expected_literal = ";"},
      {.expected_type = TokenType::RBRACE, .expected_literal = "}"},
      {.expected_type = TokenType::ELSE, .expected_literal = "else"},
      {.expected_type = TokenType::LBRACE, .expected_literal = "{"},
      {.expected_type = TokenType::RETURN, .expected_literal = "return"},
      {.expected_type = TokenType::FALSE, .expected_literal = "false"},
      {.expected_type = TokenType::SEMICOLON, .expected_literal = ";"},
      {.expected_type = TokenType::RBRACE, .expected_literal = "}"},
      {.expected_type = TokenType::STRING, .expected_literal = "foobar"},
      {.expected_type = TokenType::STRING, .expected_literal = "foo bar"},
      {.expected_type = TokenType::LBRACKET, .expected_literal = "["},
      {.expected_type = TokenType::INT, .expected_literal = "1"},
      {.expected_type = TokenType::COMMA, .expected_literal = ","},
      {.expected_type = TokenType::INT, .expected_literal = "2"},
      {.expected_type = TokenType::COMMA, .expected_literal = ","},
      {.expected_type = TokenType::INT, .expected_literal = "3"},
      {.expected_type = TokenType::RBRACKET, .expected_literal = "]"},
      {.expected_type = TokenType::SEMICOLON, .expected_literal = ";"},
      {.expected_type = TokenType::FOR, .expected_literal = "for"},
      {.expected_type= TokenType::LPAREN, .expected_literal = "("},
      {.expected_type = TokenType::RPAREN, .expected_literal = ")"},
      {.expected_type = TokenType::LBRACE, .expected_literal = "{"},
      {.expected_type = TokenType::RBRACE, .expected_literal = "}"},
      {.expected_type = TokenType::EOI, .expected_literal = ""},
  };

  Lexer lex = Lexer(input);
  for (size_t i = 0; i < tests.size(); i++) {
    Test test = tests[i];
    std::shared_ptr<Token> tok = lex.NextToken();
    TokenType expected_type = test.expected_type;
    std::string expected_literal = test.expected_literal;

    if (tok->GetType() != expected_type) {
      std::cout << "test[" << i << "]: wrong TokenType. expected: " << static_cast<int>(expected_type)
          << ", actual: " << static_cast<int>(tok->GetType()) << "\n";
      return;
    }

    if (expected_literal.compare(tok->GetLiteral()) != 0) {
      std::cout << "test[" << i << "]: wrong literal. expected: " << expected_literal
          << ", actual: " << tok->GetLiteral() << "\n";
      return;
    }

  }
  std::cout << "TestNextToken() passed\n";
}

int main() {
  TestNextToken();

  return 0;
}
