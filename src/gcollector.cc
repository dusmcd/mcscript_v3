#include <gcollector.h>

void GCollector::Collect() {
  for (auto& obj : objects_) {
    if (obj->Type() != ObjectType::BOOLEAN_OBJ && obj->Type() != ObjectType::NULL_OBJ) {
      delete(obj);
    }
  }

  objects_.clear();
}

void GCollector::TrackObject(::Object* obj) {
  objects_.push_back(obj);
}

GCollector::~GCollector() {
  // clean up any remaining objects (should only be the static)
  for (auto& obj : objects_) {
    delete (obj);
  }

  objects_.clear();
}