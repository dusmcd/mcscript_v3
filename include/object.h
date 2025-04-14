#ifndef MCSCRIPT_V3_OBJECT_H
#define MCSCRIPT_V3_OBJECT_H

#include <string>

enum class ObjectType : int {
  INTEGER_OBJ,
  BOOLEAN_OBJ,
  NULL_OBJ
};

class Object {
  public:
    virtual std::string Inspect() const = 0;
    virtual ObjectType Type() const = 0;
    virtual ~Object() {}
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


#endif