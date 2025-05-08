#include <object.h>
#include <iostream>


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
    return new Error("len function only takes one argument");
  }
  Object* obj = args[0];
  switch(obj->Type()) {
    case ObjectType::STRING_OBJ: {
      String* str = dynamic_cast<String*>(obj);
      return new Integer(str->GetValue().size());
    }
    case ObjectType::ARRAY_OBJ: {
      Array* arr = dynamic_cast<Array*>(obj);
      return new Integer(arr->GetElements()->size());
    }
    default: {
      std::string msg = "unrecognized type: " + Object::ObjectTypeStr(obj->Type());
      return new Error(msg);
    }
  }

}

Object* Push(std::vector<Object*> args) {
  if (args.size() != 2) {
    return new Error(std::string("push function only takes 2 arguments"));
  }

  Array* arr = dynamic_cast<Array*>(args[0]);
  if (arr == nullptr) {
    return new Error(std::string("expecting array as first argument"));
  }

  Object* obj = args[1];
  arr->GetElements()->push_back(obj);
  obj->AddRef();

  return nullptr;
}


Object* Print(std::vector<Object*> args) {
  for (size_t i = 0; i < args.size(); i++) {
    Object* arg = args[i];
    if (arg != nullptr) {
      std::cout << arg->Inspect();
    }

    if (i < args.size() - 1) {
      std::cout << " ";
    }
  }

  std::cout << std::endl;

  return nullptr;
}



std::unordered_map<std::string, BuiltIn*> GetBuiltIns() {
  std::unordered_map<std::string, BuiltIn*> result = {
    {"len", new BuiltIn(Length)},
    {"push", new BuiltIn(Push)},
    {"print", new BuiltIn(Print)}
  };

  return result;
}


std::string Array::Inspect() const {
  std::string result = "[";

  for (size_t i = 0; i < objs_->size(); i++) {
    Object* obj = (*objs_)[i];
    result.append(obj->Inspect());
    if (i < objs_->size() - 1) {
      result.append(", ");
    }
  }

  result.append("]");

  return result;
}
