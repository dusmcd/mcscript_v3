#ifndef MCSCRIPT_V3_OBJECT_H
#define MCSCRIPT_V3_OBJECT_H

#include <string>

enum class ObjectType : int {
  INTEGER_OBJ,
  BOOLEAN_OBJ,
  NULL_OBJ,
  RETURN_VALUE_OBJ,
  ERROR_OBJ
};

class Object {
  public:
    virtual std::string Inspect() const = 0;
    virtual ObjectType Type() const = 0;
    virtual ~Object() {}
    static std::string ObjectTypeStr(ObjectType type);
};

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
      // empty
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
      // empty
    }

    inline ObjectType Type() const override {
      return ObjectType::ERROR_OBJ;
    }

    inline std::string Inspect() const override {
      std::string result = "ERROR";
      result.append(message_);

      return result;
    }

    inline std::string GetMessage() const {
      return message_;
    }

  private:
    std::string message_;
};


#endif // MCSCRIPT_V3_OBJECT_H