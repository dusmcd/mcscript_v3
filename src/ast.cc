#include <ast.h>

std::string Program::String() const {
  for (auto stmt : statements_) {
    return stmt->String();
  }
  return "";
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