#ifndef MCSCRIPT_V3_ENVIRONMENT_H
#define MCSCRIPT_V3_ENVIRONMENT_H

#include <object.h>
#include <string>
#include <unordered_map>

class Environment {
  public:
    // constructor
    Environment(){}
    Environment(const std::shared_ptr<Environment> outer) : outer_(outer) {
      // empty
    }

    Object* Get(std::string name);
    inline void Set(std::string name, Object* val) {
      store_[name] = val;
    }



  private:
    std::unordered_map<std::string, ::Object*> store_;
    const std::shared_ptr<Environment> outer_;
};


#endif // MCSCRIPT_V3_ENVIRONMENT_H