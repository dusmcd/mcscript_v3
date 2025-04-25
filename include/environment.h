#ifndef MCSCRIPT_V3_ENVIRONMENT_H
#define MCSCRIPT_V3_ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <memory>

template <typename T>
class Environment {
  public:
    // constructor
    Environment(){}
    Environment(const std::shared_ptr<Environment> outer) : outer_(outer) {
      // empty
    }

    inline T Get(std::string name) {
      if (store_.count(name) == 0) {
        return nullptr;
      }

      return store_.at(name);
    }

    inline void Set(std::string name, T val) {
      store_[name] = val;
    }



  private:
    std::unordered_map<std::string, T> store_;
    const std::shared_ptr<Environment> outer_;
};


#endif // MCSCRIPT_V3_ENVIRONMENT_H