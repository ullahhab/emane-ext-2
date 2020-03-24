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
      pImpl_{new Impelementation{serialization}}{}
      
      EMANE::Events::HBScheduleEvent::~HBScheduleEvent(){}
      
      
      bool EMANE::Event::HBScheduleEvent::isSlotStructured(){
        return pImpl->isSlotStructured();
      }
      
      
     const EMANE::Events::HBScheduleEvent::weights & EMANE::Events::HBScheduleEvent::getWeights() const
     {
       return pImpl_->getWeights();
     }
     
     const EMANE::Events::HBScheduleEvent::betas & EMANE::Events::HBScheduleEvent::getBetas() const {
       return pImpl->getBetas();
     }
       
        
      
