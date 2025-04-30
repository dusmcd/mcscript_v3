#include <object.h>

std::string Object::ObjectTypeStr(ObjectType type) {
  switch(type) {
    case ObjectType::INTEGER_OBJ:
      return "INTEGER";
    case ObjectType::BOOLEAN_OBJ:
      return "BOOLEAN";
    case ObjectType::NULL_OBJ:
      return "NULL_T";
    case ObjectType::RETURN_VALUE_OBJ:
      return "RETURN_VALUE";
    case ObjectType::ERROR_OBJ:
      return "ERROR";
    case ObjectType::STRING_OBJ:
      return "STRING";
    default:
      return "UNRECOGNIZED TYPE";
  }
}


std::string Function::Inspect() const {
  std::string result = "function(";

  for (size_t i = 0; i < params_.size(); i++) {
    result.append(params_[i]->String());
    if (i < params_.size() - 1) {
      result.append(", ");
    }
  }

  result.append(") {\n");
  result.append(body_->String());
  result.append("\n}");

  return result;
}

Object* Length(std::vector<Object*> args) {
  if (args.size() != 1) {
    return nullptr;
  }
  Object* obj = args[0];
  if (obj->Type() != ObjectType::STRING_OBJ) {
    return nullptr;
  }

  String* str = dynamic_cast<String*>(obj);
  return new Integer(str->GetValue().size());
}



std::unordered_map<std::string, BuiltIn*> GetBuiltIns() {
  std::unordered_map<std::string, BuiltIn*> result = {
    {"len", new BuiltIn(Length)}
  };

  return result;
}