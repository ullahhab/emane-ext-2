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
    using weight = std::set<unit64_t>;
    HBScheduleEvent(const Serialization & serialization);
    bool isSlot() const;
    const weight & getWeight const;
    float beta & getBeta();
    };
  }
}
    
    
    
