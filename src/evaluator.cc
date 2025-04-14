#include <evaluator.h>
#include <typeinfo>
#include <cxxabi.h>
#include <stdlib.h>

std::shared_ptr<::Object> Eval(std::shared_ptr<::Node> node) {
  const std::string typeName = GetTypeName(node);
  if (typeName.size() == 0) {
    return nullptr;
  }

  if (typeName.compare("Program") == 0) {
    // evaluate statements
    auto program = std::dynamic_pointer_cast<::Program>(node);
    return EvalStatements(program->GetStatements());
  }
  else if (typeName.compare("ExpressionStatement") == 0) {
    // evaluate expression
    auto es = std::dynamic_pointer_cast<::ExpressionStatement>(node);
    return Eval(es->GetExpression());
  }

  // evaluate expressions
  else if (typeName.compare("IntegerLiteral") == 0) {
    auto exp = std::dynamic_pointer_cast<::IntegerLiteral>(node);
    auto obj = std::make_shared<::Integer>(exp->GetValue());
    return obj;
  }

  else if (typeName.compare("BooleanExpression") == 0) {
    auto exp = std::dynamic_pointer_cast<::BooleanExpression>(node);
    auto obj = exp->GetValue() ? TRUE : FALSE; 
    return obj;
  }

  else if (typeName.compare("PrefixExpression") == 0) {
    auto exp = std::dynamic_pointer_cast<::PrefixExpression>(node);
    std::shared_ptr<::Object> right = Eval(exp->GetRight());
    return EvalPrefixExpression(exp->TokenLiteral(), right);
  }

  return nullptr;
}


std::shared_ptr<::Object> EvalPrefixExpression(std::string op, std::shared_ptr<::Object> right) {
  if (op.compare("!") == 0) {
    return EvalBangExpression(right);
  }

  return NULL_T;
}

std::shared_ptr<::Object> EvalBangExpression(std::shared_ptr<::Object> right) {
  if (right == TRUE) {
    return FALSE;
  }
  else if (right == FALSE) {
    return TRUE;
  }
  else if (right == NULL_T) {
    return TRUE;
  }
  else {
    return FALSE;
  }

}


std::string GetTypeName(std::shared_ptr<::Node> node) {
  const char* mangled = typeid(*node).name();
  int status;
  char* demangled = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
  if (status != 0) {
    return "";
  }
  std::string result(demangled);

  free(demangled);
  return result;
}

std::shared_ptr<::Object> EvalStatements(std::vector<std::shared_ptr<::Statement>> stmts) {
  for (const auto& stmt : stmts) {
    return Eval(stmt);
  }

  return nullptr;
}