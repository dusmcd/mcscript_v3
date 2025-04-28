#include <gcollector.h>
#include <algorithm>

void GCollector::Collect() {
  for (auto& obj : objects_) {
    if (obj == nullptr) {
      continue;
    }
    if (obj->Type() != ObjectType::BOOLEAN_OBJ 
        && obj->Type() != ObjectType::NULL_OBJ 
        && obj->IsNotReferenced()) {
      delete obj;
      obj = nullptr;
    }
  }

  objects_.erase(std::remove(objects_.begin(), objects_.end(), nullptr), objects_.end());
}

void GCollector::CollectAll() {
  for (auto& obj : objects_) {
    if (obj == nullptr) {
      continue;
    }
    if (obj->Type() != ObjectType::BOOLEAN_OBJ && obj->Type() != ObjectType::NULL_OBJ) {
      delete obj;
    }
  }

  objects_.clear();
}

void GCollector::TrackObject(::Object* obj) {
  objects_.push_back(obj);
}
