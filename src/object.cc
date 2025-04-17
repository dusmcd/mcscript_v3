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
    default:
      return "UNRECOGNIZED TYPE";
  }
}