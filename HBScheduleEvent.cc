#include "HBscheduleEvent.h"

#include <cstdint>
#include <tuple>

class EMANE::Events::HBScheduleEvent::Implementation
{
  Implementation(const Serilizaion & serialization):
    bHasStructure_{}
    {
      EMANEMessage::HBScheduleEvent msg{};
      
