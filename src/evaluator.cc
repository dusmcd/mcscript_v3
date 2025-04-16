#include <evaluator.h>
#include <typeinfo>
#include <cxxabi.h>
#include <stdlib.h>

// destructor
Evaluator::~Evaluator() {
  delete TRUE_;
  delete FALSE_;
  delete NULL_T_;
}

Object* Evaluator::Eval(std::shared_ptr<::Node> node) {
  const std::string typeName = GetTypeName_(node);
  if (typeName.size() == 0) {
    return nullptr;
  }

  if (typeName.compare("Program") == 0) {
    // evaluate statements
    auto program = std::dynamic_pointer_cast<::Program>(node);
    return EvalProgram_(program);
  }
  else if (typeName.compare("ExpressionStatement") == 0) {
    // evaluate expression
    auto es = std::dynamic_pointer_cast<::ExpressionStatement>(node);
    return Eval(es->GetExpression());
  }
  else if (typeName.compare("BlockStatement") == 0) {
    auto block = std::dynamic_pointer_cast<::BlockStatement>(node);
    return EvalBlockStatement_(block);
  }

  else if (typeName.compare("ReturnStatement") == 0) {
    auto rs = std::dynamic_pointer_cast<ReturnStatement>(node);
    Object* value = Eval(rs->GetReturnVal());
    return new ReturnValue(value);
  }

  // evaluate expressions
  else if (typeName.compare("IntegerLiteral") == 0) {
    auto exp = std::dynamic_pointer_cast<::IntegerLiteral>(node);
    Object* obj = new Integer(exp->GetValue());
    return obj;
  }

  else if (typeName.compare("BooleanExpression") == 0) {
    auto exp = std::dynamic_pointer_cast<::BooleanExpression>(node);
    Boolean* obj = NativeBooleanToBooleanObj_(exp->GetValue());
    return obj;
  }

  else if (typeName.compare("PrefixExpression") == 0) {
    auto exp = std::dynamic_pointer_cast<::PrefixExpression>(node);
    ::Object* right = Eval(exp->GetRight());
    return EvalPrefixExpression_(exp->TokenLiteral(), right);
  }

  else if (typeName.compare("InfixExpression") == 0) {
    auto exp = std::dynamic_pointer_cast<::InfixExpression>(node);
    ::Object* left = Eval(exp->GetLeft());
    ::Object* right = Eval(exp->GetRight());
    return EvalInfixExpression_(exp->GetOp(), left, right);
  }

  else if (typeName.compare("IfExpression") == 0) {
    auto ie = std::dynamic_pointer_cast<IfExpression>(node);
    return EvalIfExpression_(ie);
  }

  return nullptr;
}

Object* Evaluator::EvalIfExpression_(std::shared_ptr<IfExpression> ie) {
  Object* condition = Eval(ie->GetCondition());
  TrackObject(condition);

  if (IsTruthy_(condition)) {
    return Eval(ie->GetConsequence());
  } else if (ie->GetAlternative() != nullptr) {
    return Eval(ie->GetAlternative());
  }

  return NULL_T();
}


bool Evaluator::IsTruthy_(Object* condition) {
  if (condition == TRUE()) {
    return true;
  } else if (condition == FALSE()) {
    return false;
  } else if (condition == NULL_T()) {
    return false;
  }

  return true;
}

Object* Evaluator::EvalInfixExpression_(std::string op, Object* left, Object* right) {
  if (left->Type() == ObjectType::INTEGER_OBJ && right->Type() == ObjectType::INTEGER_OBJ) {
    return EvalIntegerInfixExpression_(op, left, right);
  } else if (op.compare("==") == 0) {
    return NativeBooleanToBooleanObj_(left == right);
  } else if (op.compare("!=") == 0) {
    return NativeBooleanToBooleanObj_(left != right);
  }

  return NULL_T();
}

Object* Evaluator::EvalIntegerInfixExpression_(std::string op, Object* left, Object* right) {
  long leftVal = dynamic_cast<Integer*>(left)->GetValue();
  long rightVal = dynamic_cast<Integer*>(right)->GetValue();

  delete left;
  delete right;
  left = nullptr; 
  right = nullptr;

  if (op.compare("+") == 0) {
    return new Integer(leftVal + rightVal);
  } else if (op.compare("-") == 0) {
    return new Integer(leftVal - rightVal);
  } else if (op.compare("*") == 0) {
    return new Integer(leftVal * rightVal);
  } else if (op.compare("/") == 0) {
    return new Integer(leftVal / rightVal);
  } else if (op.compare("<") == 0) {
    return NativeBooleanToBooleanObj_(leftVal < rightVal);
  } else if (op.compare(">") == 0) {
    return NativeBooleanToBooleanObj_(leftVal > rightVal);
  } else if (op.compare("==") == 0) {
    return NativeBooleanToBooleanObj_(leftVal == rightVal);
  } else if (op.compare("!=") == 0) {
    return NativeBooleanToBooleanObj_(leftVal != rightVal);
  }

  return NULL_T();
}


Boolean* Evaluator::NativeBooleanToBooleanObj_(bool input) {
  if (input) {
    return TRUE();
  }
  return FALSE();
}


::Object* Evaluator::EvalPrefixExpression_(std::string op, ::Object* right) {
  if (op.compare("!") == 0) {
    return EvalBangExpression_(right);
  }
  if (op.compare("-") == 0) {
    return EvalMinusExpression_(right);
  }

  return NULL_T();
}

Object* Evaluator::EvalMinusExpression_(Object* right) {
  if (right->Type() != ObjectType::INTEGER_OBJ) {
    return nullptr;
  }

  auto obj = dynamic_cast<Integer*>(right);
  obj->SetValue(obj->GetValue() * -1);

  return obj;
}

::Object* Evaluator::EvalBangExpression_(::Object* right) {
  if (right == TRUE()) {
    return FALSE();
  }
  else if (right == FALSE()) {
    return TRUE();
  }
  else if (right == NULL_T()) {
    return TRUE();
  }
  else {
    return FALSE();
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

Object* Evaluator::EvalProgram_(std::shared_ptr<Program> program) {
  Object* result = nullptr;
  for (const auto& stmt : program->GetStatements()) {
    result = Eval(stmt);

    auto returnValue = dynamic_cast<ReturnValue*>(result);
    if (returnValue != nullptr) {
      TrackObject(returnValue);
      return returnValue->GetValue();
    }
  }

  return result;
}

Object* Evaluator::EvalBlockStatement_(std::shared_ptr<BlockStatement> block) {
  Object* result = nullptr;
  for (const auto& stmt : block->GetStatements()) {
    result = Eval(stmt);

    auto returnValue = dynamic_cast<ReturnValue*>(result);
    if (returnValue != nullptr) {
      return returnValue;
    }
  }

  return result;
}
