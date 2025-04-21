#include <environment.h>

Object* Environment::Get(std::string name) {
  if (store_.count(name) == 0) {
    return nullptr;
  }

  return store_.at(name);
}