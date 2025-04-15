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
    void TrackObject(Object* obj);




  private:
    GCollector() {}
    std::vector<::Object*> objects_;
};



#endif // MCSCRIPT_V3_GCOLLECTOR_H