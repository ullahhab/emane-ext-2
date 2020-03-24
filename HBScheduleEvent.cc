#include "HBscheduleEvent.h"

#include <cstdint>
#include <tuple>

class EMANE::Events::HBScheduleEvent::Implementation
{
  Implementation(const Serilizaion & serialization):
    bHasStructure_{}
    {
      EMANEMessage::HBScheduleEvent msg{};
      if(!msg.ParseFromString(serialization))
      {
        throw SerialationException("unable to deserialize : HBScheduleEvent");
      }
      
     //TODO: Confirm what should a scheduler be doing exactly? Also that is something that would come along with some development
      
      if(msg.has_structure())
      {
        bHasStructure_ = true;
      }
      
      
      
      Emane::Events::HBScheduleEvent::HBScheduleEvent(const Serilization & serialization):
      Event{IDENTIFIER},
      pImpl_{new Impelementation{serialization}}{}
      
      EMANE::Events::HBScheduleEvent::~HBScheduleEvent(){}
      
      
      bool EMANE::Event::is
     
        
      
