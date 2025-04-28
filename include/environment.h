#ifndef MCSCRIPT_V3_ENVIRONMENT_H
#define MCSCRIPT_V3_ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include <memory>

template <typename T>
class Environment {
  public:
    // constructor
    Environment() : outer_(nullptr) {}
    Environment(const std::shared_ptr<Environment> outer) : outer_(outer) {
      // empty
    }

    inline T Get(std::string name) {
      if (store_.count(name) == 0) {
        if (outer_ != nullptr) {
          return outer_->Get(name);
        }
        else {
          return nullptr;
        }
      }

      return store_.at(name);
    }

    inline void Set(std::string name, T val) {
      store_[name] = val;
    }

    inline std::unordered_map<std::string, T> GetStore() const {
      return store_;
    }



  private:
    std::unordered_map<std::string, T> store_;
    const std::shared_ptr<Environment> outer_;
};


#endif // MCSCRIPT_V3_ENVIRONMENT_H