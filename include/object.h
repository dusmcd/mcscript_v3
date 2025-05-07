#ifndef MCSCRIPT_V3_OBJECT_H
#define MCSCRIPT_V3_OBJECT_H

#include <string>
#include <vector>
#include <memory>
#include <ast.h>
#include <environment.h>
#include <functional>
#include <unordered_map>

enum class ObjectType : int {
  INTEGER_OBJ,
  BOOLEAN_OBJ,
  NULL_OBJ,
  RETURN_VALUE_OBJ,
  ERROR_OBJ,
  FUNCTION_OBJ,
  STRING_OBJ,
  BUILT_IN_OBJ,
  ARRAY_OBJ
};

class Object {
  public:
    virtual std::string Inspect() const = 0;
    virtual ObjectType Type() const = 0;
    virtual ~Object() {}
    static std::string ObjectTypeStr(ObjectType type);

    inline void AddRef() {
      refCount_++;
    }

    inline void SubtractRef() {
      if (refCount_ > 0)
        refCount_--;
    }

    inline bool IsNotReferenced() {
      return refCount_ == 0; 
    }

  protected:
    size_t refCount_;
};

using BuiltInFunction = std::function<Object*(std::vector<Object*>)>;


class Null : public Object {
  public:
    inline std::string Inspect() const override {
      return "null";
    }

    inline ObjectType Type() const override {
      return ObjectType::NULL_OBJ;
    }
};

class Integer : public Object {
  public:
    Integer(long value) : value_(value) {
      //empty
      refCount_ = 0;
    }

    inline std::string Inspect() const override {
      return std::to_string(value_);
    }

    inline ObjectType Type() const override {
      return ObjectType::INTEGER_OBJ;
    }

    inline void SetValue(long value) {
      value_ = value;
    }

    inline long GetValue() const {
      return value_;
    }

  private:
    long value_;
};

class Boolean : public Object {
  public:
    Boolean(bool value) : value_(value) {
      // empty
    }

    inline std::string Inspect() const override {
      return value_ ? "true" : "false";
    }

    inline ObjectType Type() const override {
      return ObjectType::BOOLEAN_OBJ;
    }

    inline bool GetValue() const {
      return value_;
    }

  private:
    bool value_;
};

class ReturnValue : public Object {
  public:
    ReturnValue(Object* value) : value_(value) {
      refCount_ = 0;
    }

    inline std::string Inspect() const override {
      return value_->Inspect();
    }

    inline ObjectType Type() const override {
      return ObjectType::RETURN_VALUE_OBJ;
    }

    inline Object* GetValue() const {
      return value_;
    }

  private:
    Object* value_;
};

class Error : public Object {
  public:
    Error(std::string msg) : message_(msg) {
      refCount_ = 0;
    }

    inline ObjectType Type() const override {
      return ObjectType::ERROR_OBJ;
    }

    inline std::string Inspect() const override {
      std::string result = "ERROR: ";
      result.append(message_);

      return result;
    }

    inline std::string GetMessage() const {
      return message_;
    }

  private:
    std::string message_;
};

class Function : public Object {
  public:
    Function(
      std::vector<std::shared_ptr<::Identifier>> params,
      std::shared_ptr<BlockStatement> body,
      std::shared_ptr<Environment<Object*>> env) : params_(params), body_(body), env_(env) {
        refCount_ = 0;
      }
    
    inline std::vector<std::shared_ptr<::Identifier>> GetParams() const {
      return params_;
    }

    inline std::shared_ptr<BlockStatement> GetBody() const {
      return body_;
    }

    inline std::shared_ptr<Environment<Object*>> GetEnv() const {
      return env_;
    }

    inline ObjectType Type() const override {
      return ObjectType::FUNCTION_OBJ;
    }

    std::string Inspect() const override;
    
  private:
      std::vector<std::shared_ptr<::Identifier>> params_;
      std::shared_ptr<BlockStatement> body_;
      std::shared_ptr<Environment<Object*>> env_;
};

class String : public Object {
  public:
    String(std::string value) : value_(value) {
      // empty
    }
    
    inline ObjectType Type() const override {
      return ObjectType::STRING_OBJ;
    }

    inline std::string Inspect() const override {
      return value_;
    }

    inline std::string GetValue() const {
      return value_;
    }

  private:
    std::string value_;
};

class BuiltIn : public Object {
  public:
    BuiltIn(BuiltInFunction fn) : fn_(fn) {
      refCount_ = 0;
    }

    inline ObjectType Type() const override {
      return ObjectType::BUILT_IN_OBJ;
    }

    inline std::string Inspect() const override {
      return std::string("builtin function");
    }

    inline BuiltInFunction GetFunc() const {
      return fn_;
    }

  private:
    BuiltInFunction fn_;
};

class Array : public Object {
  public:
    Array() {
      objs_ = std::make_shared<std::vector<Object*>>();
    }
    inline void AddObj(Object* obj) {
      objs_->push_back(obj);
    }

    inline ObjectType Type() const override {
      return ObjectType::ARRAY_OBJ;
    }

    inline std::shared_ptr<std::vector<Object*>> GetElements() const {
      return objs_;
    }

    std::string Inspect() const override;


  private:
    std::shared_ptr<std::vector<Object*>> objs_;
};

/*
===============================================
BUILT IN FUNCTIONS
===============================================
*/
Object* Length(std::vector<Object*> args);
Object* Push(std::vector<Object*> args);  

std::unordered_map<std::string, BuiltIn*> GetBuiltIns();

#endif // MCSCRIPT_V3_OBJECT_H
