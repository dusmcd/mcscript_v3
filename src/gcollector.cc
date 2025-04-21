#include <gcollector.h>

void GCollector::Collect() {
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
