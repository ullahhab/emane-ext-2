#include "emane/event.h"
#include "emane/events/eventsids.h"
#include "emane/events/slotinfo.h"
#include "emane/events/slotstructure.h"



#include <memory>

namespace EMANE
{
  namespace Events
   {
    class HBScheduleEvent: public Event
    {
    public:
    using weights = std::set<unit64_t>;
    HBScheduleEvent(const Serialization & serialization);
    const weight & getWeight() const;
    float beta & getBeta() const;
    bool isSlotStructured();
    std::pair<const SlotStructure &, bool> getSlotStructure() const;
    private:
      class Implementation;
      std::unique_ptr<Implementation> pImpl_;
    };
  }
}
    
    
    
