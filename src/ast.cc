#include <ast.h>

std::string Program::String() const {
  std::string result = "";
  for (auto stmt : statements_) {
    result.append(stmt->String());
  }
  return result;
}

std::string Identifier::String() const {
  char buff[256];

  snprintf(buff, sizeof(buff), "%s", value_.c_str());

  std::string str(buff);

  return str;
}

std::string ExpressionStatement::String() const {
  if (expression_ != nullptr) {
    return expression_->String();
  }
  return "";
}

std::string VarStatement::String() const {
  char buff[256];

  snprintf(buff, sizeof(buff), "%s %s = %s;",
          TokenLiteral().c_str(), name_->String().c_str(),
           value_->String().c_str());
  std::string str(buff);

  return str;
}

std::string ReturnStatement::String() const {
  char buff[256];

  snprintf(buff, sizeof(buff), "%s %s;", TokenLiteral().c_str(), 
            return_value_->String().c_str());
  std::string str(buff);

  return str;
}

std::string PrefixExpression::String() const {
  char buff[256];
  snprintf(buff, sizeof(buff), "(%s%s)",
      op_.c_str(), right_->String().c_str());
  
  std::string str(buff);

  return str;
}

std::string InfixExpression::String() const {
  char buff[256];
  snprintf(buff, sizeof(buff), "(%s %s %s)",
      left_->String().c_str(), op_.c_str(),
       right_->String().c_str());
  
  std::string str(buff);
  return str;
}

std::string BlockStatement::String() const {
  std::string str = "";
  for (const auto& stmt : statements_) {
    str.append(stmt->String());
  }

  return str;
  
}

std::string IfExpression::String() const {
  std::string str = "if ";
  str.append(condition_->String());
  str.append("{ ");
  str.append(consequence_->String());
  str.append(" }");

  if (alternative_ != nullptr) {
    str.append(" else ");
    str.append("{ ");
    str.append(alternative_->String());
    str.append(" }");
  }

  return str;
}

std::string FunctionLiteral::String() const {
  std::string str = "";
  str.append(token_->GetLiteral());
  str.append("(");
  for (size_t i = 0; i < parameters_.size(); i++) {
    str.append(parameters_[i]->String());
    if (i < parameters_.size() - 1) {
      str.append(", ");
    } else {
      str.append(") ");
    }
  }

  str.append("{ ");
  str.append(body_->String());
  str.append(" };");

  return str;
}

std::string CallExpression::String() const {
  std::string str = "";
  str.append(func_->String());
  str.append("(");

  for (size_t i = 0; i < args_.size(); i++) {
    std::shared_ptr<Expression> arg = args_[i];
    str.append(arg->String());
    if (i < args_.size() - 1) {
      str.append(", ");
    }  
  }
  str.append(");");
  return str;
}



std::string ArrayLiteral::String() const {
  std::string result = "[";

  for (size_t i = 0; i < exps_.size(); i++) {
    result.append(exps_[i]->String());
    if (i < exps_.size() - 1) {
      result.append(",");
    }
  }

  result.append("]");

  return result;
}


std::string IndexExpression::String() const {
  std::string result = exp_->String();
  result += '[';
  result.append(idx_->String());
  result += ']';
  return result;
}

std::string AssignExpression::String() const {
  std::string result = ident_->String();
  result.append(" = " + newVal_->String());

  return result;
}

std::string ForStatement::String() const {
  std::string result = "for(";
  result.append(varStmt_->String());
  result.append("; ");

  result.append(condition_->String());
  result.append("; ");

  result.append(afterAction_->String());
  result.append(") {\n");

  result.append(block_->String());
  result.append("\n}");

  return result;
}
