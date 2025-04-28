#ifndef MCSCRIPT_V3_GCOLLECTOR_H
#define MCSCRIPT_V3_GCOLLECTOR_H

#include <vector>
#include <object.h>

// singleton class
// garbage collector
class GCollector {
  public:
    inline static GCollector& getGCollector() {
      static GCollector gCollector;
      return gCollector;
    }


    GCollector(const GCollector&) = delete;
    GCollector& operator=(const GCollector&) = delete;

    void Collect();

    // this cleans up all objects that are not null regardless of how many references (called when program terminates)
    void CollectAll();
    void TrackObject(Object* obj);
    inline size_t GetNumObjects() const {
      return objects_.size();
    }



  private:
    GCollector() {}
    std::vector<::Object*> objects_;
};



#endif // MCSCRIPT_V3_GCOLLECTOR_H