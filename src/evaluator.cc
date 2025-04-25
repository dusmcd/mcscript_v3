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

Object* Evaluator::Eval(std::shared_ptr<::Node> node, std::shared_ptr<Environment<Object*>> env) {
  const std::string typeName = GetTypeName_(node);
  if (typeName.size() == 0) {
    return nullptr;
  }

  if (typeName.compare("Program") == 0) {
    // evaluate statements
    auto program = std::dynamic_pointer_cast<::Program>(node);
    return EvalProgram_(program, env);
  }
  else if (typeName.compare("ExpressionStatement") == 0) {
    // evaluate expression
    auto es = std::dynamic_pointer_cast<::ExpressionStatement>(node);
    return Eval(es->GetExpression(), env);
  }
  else if (typeName.compare("BlockStatement") == 0) {
    auto block = std::dynamic_pointer_cast<::BlockStatement>(node);
    return EvalBlockStatement_(block, env);
  }

  else if (typeName.compare("VarStatement") == 0) {
    auto stmt = std::dynamic_pointer_cast<::VarStatement>(node);
    Object* val = Eval(stmt->GetValue(), env);
    if (IsError_(val)) {
      return val;
    }
    env->Set(stmt->GetName()->GetValue(), val);
    return nullptr;
  }

  else if (typeName.compare("ReturnStatement") == 0) {
    auto rs = std::dynamic_pointer_cast<ReturnStatement>(node);
    Object* value = Eval(rs->GetReturnVal(), env);
    if (IsError_(value)) {
      return value;
    }
    return new ReturnValue(value);
  }

  // evaluate expressions
  else if (typeName.compare("Identifier") == 0) {
    auto i = std::dynamic_pointer_cast<Identifier>(node);
    return EvalIdentifier_(i->GetValue(), env);
  }
  else if (typeName.compare("FunctionLiteral") == 0) {
    auto fn = std::dynamic_pointer_cast<FunctionLiteral>(node);
    return new Function(fn->GetParameters(), fn->GetBody(), env);
  }
  else if (typeName.compare("CallExpression") == 0) {
    auto call = std::dynamic_pointer_cast<CallExpression>(node);
    Object* obj = Eval(call->GetFunc(), env);
    auto func = dynamic_cast<Function*>(obj);
    if (func == nullptr) {
      char buff[128];
      snprintf(buff, sizeof(buff), "%s is not a function", obj->Inspect().c_str());
      return  NewError_(std::string(buff));
    }

    // GARGAGE COLLECTION
    TrackObject(obj);

    std::vector<Object*> args = EvalParameters_(func, call->GetArgs());
    return EvalFunctionCall_(func, args, func->GetEnv());

  }
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
    ::Object* right = Eval(exp->GetRight(), env);
    if (IsError_(right)) {
      return right;
    }
    return EvalPrefixExpression_(exp->TokenLiteral(), right);
  }

  else if (typeName.compare("InfixExpression") == 0) {
    auto exp = std::dynamic_pointer_cast<::InfixExpression>(node);
    ::Object* left = Eval(exp->GetLeft(), env);
    if (IsError_(left)) {
      return left;
    }

    ::Object* right = Eval(exp->GetRight(), env);
    if (IsError_(right)) {
      return right;
    }
    return EvalInfixExpression_(exp->GetOp(), left, right);
  }

  else if (typeName.compare("IfExpression") == 0) {
    auto ie = std::dynamic_pointer_cast<IfExpression>(node);
    return EvalIfExpression_(ie, env);
  }

  return nullptr;
}

Object* Evaluator::EvalIfExpression_(std::shared_ptr<IfExpression> ie, std::shared_ptr<Environment<Object*>> env) {
  Object* condition = Eval(ie->GetCondition(), env);
  if (IsError_(condition)) {
    return condition;
  }
  // GARBAGE COLLECTION
  TrackObject(condition);

  if (IsTruthy_(condition)) {
    return Eval(ie->GetConsequence(), env);
  } else if (ie->GetAlternative() != nullptr) {
    return Eval(ie->GetAlternative(), env);
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

std::string Evaluator::GetInfixErrorMsg_(const char* format, Object* left, std::string op, Object* right) {
  char buff[256];
  std::string leftStr = Object::ObjectTypeStr(left->Type());
  std::string rightStr = Object::ObjectTypeStr(right->Type());
  snprintf(buff, sizeof(buff), format, leftStr.c_str(), op.c_str(), rightStr.c_str());

  return std::string(buff);
}

std::string Evaluator::GetPrefixErrorMsg_(const char* format, std::string op, Object* right) {
  char buff[256];
  std::string rightStr = Object::ObjectTypeStr(right->Type());
  snprintf(buff, sizeof(buff), format, op.c_str(), rightStr.c_str());

  return std::string(buff);
}


Object* Evaluator::EvalInfixExpression_(std::string op, Object* left, Object* right) {
  // GARBAGE COLLECTION
  TrackObject(left);
  TrackObject(right);

  if (left->Type() == ObjectType::INTEGER_OBJ && right->Type() == ObjectType::INTEGER_OBJ) {
    return EvalIntegerInfixExpression_(op, left, right);
  }

  if (op.compare("==") == 0) {
    return NativeBooleanToBooleanObj_(left == right);
  }
  if (op.compare("!=") == 0) {
    return NativeBooleanToBooleanObj_(left != right);
  }

  std::string errMsg = GetInfixErrorMsg_("unknown operator: %s %s %s", left, op, right);
  return NewError_(errMsg);
}

Object* Evaluator::EvalIntegerInfixExpression_(std::string op, Object* left, Object* right) {
  long leftVal = dynamic_cast<Integer*>(left)->GetValue();
  long rightVal = dynamic_cast<Integer*>(right)->GetValue();

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

  std::string errorMsg = GetInfixErrorMsg_("unknown operator: %s %s %s", left, op, right);
  return NewError_(errorMsg);
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

  // GARBAGE COLLECTION
  TrackObject(right);
  std::string errMsg = GetPrefixErrorMsg_("unknown operator: %s%s", op, right);
  return NewError_(errMsg);
}

Object* Evaluator::EvalMinusExpression_(Object* right) {
  if (right->Type() != ObjectType::INTEGER_OBJ) {
    // GARGAGE COLLECTION
    TrackObject(right);
    std::string errMsg = GetPrefixErrorMsg_("unknown operator: %s%s", "-", right);
    return NewError_(errMsg);
  }

  auto obj = dynamic_cast<Integer*>(right);
  obj->SetValue(obj->GetValue() * -1);

  return obj;
}

::Object* Evaluator::EvalBangExpression_(::Object* right) {
  // GARBAGE COLLECTION
  TrackObject(right);
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

Object* Evaluator::EvalProgram_(std::shared_ptr<Program> program, std::shared_ptr<Environment<Object*>> env) {
  Object* result = nullptr;
  for (const auto& stmt : program->GetStatements()) {
    result = Eval(stmt, env);
    if (result == nullptr) {
      continue;
    }

    auto returnValue = dynamic_cast<ReturnValue*>(result);
    if (returnValue != nullptr) {
      // GARBAGE COLLECTION
      TrackObject(returnValue);
      return returnValue->GetValue();
    }

    if (result->Type() == ObjectType::ERROR_OBJ) {
      return result;
    }
  }

  return result;
}

Object* Evaluator::EvalBlockStatement_(std::shared_ptr<BlockStatement> block, std::shared_ptr<Environment<Object*>> env) {
  Object* result = nullptr;
  for (const auto& stmt : block->GetStatements()) {
    result = Eval(stmt, env);

    auto returnValue = dynamic_cast<ReturnValue*>(result);
    if (returnValue != nullptr) {
      return returnValue;
    }

    if (result->Type() == ObjectType::ERROR_OBJ) {
      return result;
    }
  }

  return result;
}


Error* Evaluator::NewError_(std::string message) {
  return new Error(message);
}

bool Evaluator::IsError_(Object* obj) {
  if (obj != nullptr) {
    auto err = dynamic_cast<Error*>(obj);
    if (err != nullptr) {
      return true;
    }
    return false;
  }

  return false;
}

Object* Evaluator::EvalIdentifier_(std::string name, std::shared_ptr<Environment<Object*>> env) {
  Object* obj = env->Get(name);
  if (obj == nullptr) {
    std::string errMsg = "unexpected identifier: ";
    errMsg.append(name);
    return NewError_(errMsg);
  }

  return obj;
}


std::vector<Object*> Evaluator::EvalParameters_(Function* func, std::vector<std::shared_ptr<Expression>> params) {
  std::vector<Object*> result;
  
  for (const auto& param : params) {
    result.push_back(Eval(param, func->GetEnv()));
  }

  return result;
}

Object* Evaluator::EvalFunctionCall_(Object* obj, std::vector<Object*> args, std::shared_ptr<Environment<Object*>> outerEnv) {
  auto env = std::make_shared<Environment<Object*>>(outerEnv);
  auto function = dynamic_cast<Function*>(obj);
  std::vector<std::shared_ptr<Identifier>> params = function->GetParams();
  
  // add args to inner scope
  for (size_t i = 0; i < args.size(); i++) {
    env->Set(params[i]->GetValue(), args[i]);
  }

  Object* result = Eval(function->GetBody(), env);
  if (result != nullptr && result->Type() == ObjectType::RETURN_VALUE_OBJ) {
    auto returnVal = dynamic_cast<ReturnValue*>(result);
    return returnVal->GetValue();
  }

  return result;
}