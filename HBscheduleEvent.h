#include "emane/event.h"
#include "emane/events/eventsids.h"
#include "emane/events/slotinfo.h"
#include "emane/events/slotstructure.h"


#define EMANE_EVENT_HB_SCHEDULE 106





#include <memory>

namespace EMANE
{
  namespace Events
   {
    class HBScheduleEvent: public Event
    {
    public:
    using weights = std::set<unit64_t>;
    using frequencies = std::set<uint64_t>;
    
    ~HBScheduleEvent();
    
    HBScheduleEvent(const Serialization & serialization);
    const weights & getWeights() const;
    const Slotinfos & getSlotInfos() const;
    const Frequencies & getFrequencies() const;
    float betas & getBetas() const;
    bool isSlotStructured();
    std::pair<const SlotStructure &, bool> getSlotStructure() const;
    enum {IDENTIFIER = EMANE_EVENT_HB_SCHEDULE};
    private:
      class Implementation;
      std::unique_ptr<Implementation> pImpl_;
    };
  }
}
    
    
    
