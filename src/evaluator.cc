#include <evaluator.h>
#include <typeinfo>
#include <cxxabi.h>
#include <stdlib.h>

Object* Evaluator::Eval(std::shared_ptr<::Node> node) {
  const std::string typeName = GetTypeName_(node);
  if (typeName.size() == 0) {
    return nullptr;
  }

  if (typeName.compare("Program") == 0) {
    // evaluate statements
    auto program = std::dynamic_pointer_cast<::Program>(node);
    return EvalStatements_(program->GetStatements());
  }
  else if (typeName.compare("ExpressionStatement") == 0) {
    // evaluate expression
    auto es = std::dynamic_pointer_cast<::ExpressionStatement>(node);
    return Eval(es->GetExpression());
  }

  // evaluate expressions
  else if (typeName.compare("IntegerLiteral") == 0) {
    auto exp = std::dynamic_pointer_cast<::IntegerLiteral>(node);
    Object* obj = new Integer(exp->GetValue());
    return obj;
  }

  else if (typeName.compare("BooleanExpression") == 0) {
    auto exp = std::dynamic_pointer_cast<::BooleanExpression>(node);
    auto obj = exp->GetValue() ? TRUE : FALSE; 
    return obj;
  }

  else if (typeName.compare("PrefixExpression") == 0) {
    auto exp = std::dynamic_pointer_cast<::PrefixExpression>(node);
    ::Object* right = Eval(exp->GetRight());
    return EvalPrefixExpression_(exp->TokenLiteral(), right);
  }

  return nullptr;
}


::Object* Evaluator::EvalPrefixExpression_(std::string op, ::Object* right) {
  if (op.compare("!") == 0) {
    return EvalBangExpression_(right);
  }

  return NULL_T;
}

::Object* Evaluator::EvalBangExpression_(::Object* right) {
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


std::string Evaluator::GetTypeName_(std::shared_ptr<::Node> node) {
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

::Object* Evaluator::EvalStatements_(std::vector<std::shared_ptr<::Statement>> stmts) {
  for (const auto& stmt : stmts) {
    return Eval(stmt);
  }

  return nullptr;
}