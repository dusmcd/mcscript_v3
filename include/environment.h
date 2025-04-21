#ifndef MCSCRIPT_V3_ENVIRONMENT_H
#define MCSCRIPT_V3_ENVIRONMENT_H

#include <object.h>
#include <string>
#include <unordered_map>

class Environment {
  public:
    static Environment& NewEnvironment() {
      static Environment env;
      return env;
    }

    Object* Get(std::string name);
    inline void Set(std::string name, Object* val) {
      store_[name] = val;
    }



  private:
    // constructor
    Environment() {}
    std::unordered_map<std::string, ::Object*> store_;
};


#endif // MCSCRIPT_V3_ENVIRONMENT_H