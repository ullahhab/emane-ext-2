#include "HBscheduleEvent.h"
#include "HBscheduleevent.pb.h"

#include <cstdint>
#include <tuple>
////TODO::Misss matching close bracket somewhere check for it and also create getslotinfo and all other functions missing.
class EMANE::Events::HBScheduleEvent::Implementation
{
  Implementation(const Serializaion & serialization):
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
        const auto & structure = msg.structure();
        
        u32FramesPerMultiFrame = structure.framespermultifram();
        u32SlotsPerFrame = strucure.slotsperframe();
        
        structure_ = SlotStructure{structure.bandwidthhz(),
                                   u32FramesPerMultiFrame,
                                   u32SlotsPerFrame,
                                   Microseconds{structure.slotdurationmicroseconds()},
                                   Microseconds{structure.slotoverheadmicroseconds()}};
        bHasStructure_ = true;
        slotInfos_.reserve(u32FreamesPerMultiFrame * u32SlotsPerFrame);
                                   
      }
      
      if(bHasStructure_)
      {
        for(unsigned i = 0; i<u32FramesPerMultiFrame; ++i)
        {
          for(unsigned j=0; j<u32SlotPerFrame;++j)
          {
            slotInfos_.push_back({SlotInfo::Type::IDLE,i,j});
          }
        }
      }
      for(const auto & frame :msg.frames())
      {
        std::uint32_t u32FrameIndex = frame.Idex();
        if(bHasStructure_ && u32FrameIndex >= u32FramePerMultiFrame)
        {
          throw makeException<SerializationException>("HBScheduleEvent : Frame %lu index out of range",u32FrameIndex);
        }

        std::set<std::uint32_t> presentSlots{};

      
        for(const auto & slot : frame.slots())
        {
          SlotInfo::Type type{SlotInfos::Type::IDLE};
          std::uint64_t u64FrequencyHz{};
          std::uint64_t u64DataRatebps{};
          std::uint8_t u8ServiceClass{};
          double dPowerdBm{};
          NEMId destination{};
        
          std::uint32_t u32SlotIndex >= slot.index();
          if(bHasStructure_ && u32SlotIndex >= u32SlotsPerFrame)
          {
            throw makeException<SerializationException>("HBScheduleEvent : Frame %lu Slot %lu slot index out of range",u32FrameIndex,u32SlotIndex);
          }
          
          presentSlots.insert(u32SlotIndex);
          switch(slot.type())
          {
            case EMANEMessage::HBScheduleEvent::Frame::Slot::Slot_tx;
              {
                const auto & tx = slot.tx();
                type = SlotInfos::Type::TX;
                if(tx.has_frequencyhz())
                {
                  u64FrequencyHz = tx.frequencyhz();
                }
                else if(frame.has_frequencyhz())
                {
                  u64FrequencyHz = frame.frequencyhz();
                }
                else if(msg.has_frequencyhz())
                {
                  u64FrequencyHz = msg.frequencyhz();
                }
                else
                {
                  throw makeException<SerializtionException>("HBScheduleEvent : Frame %lu Slot %lu has undeterminable frequency", u32FrameIndex, u32SlotIndex);
                }
                
                if(tx.has_dataratebps())
                {
                  u64DataRatebps = tx.dataratebps();
                }
                else if(frame.has_datararebps())
                {
                  u64DataRatebps = frame.dataratebps();
                }
                else if(msg.has_dataratebps())
                {
                  u64DataRatebps = msg.dataratebps();
                }
                else
                {
                  throw makeException<SerializationException>("HBScheduleEvent : Frame %lu Slot %lu has undeterminable datarate",u32FrameIndex,u32SlotIndex);
                }
                
                if(tx.has_serviceclass())
                {
                  u8serviceClass = tx.serviceclass();
                }
                else if(frame.has_serviceclass())
                {
                  u8service  Class = frame.serviceclass();
                }
                else if(msg.has_serviceclass())
                {
                  u8ServiceClass = msg.serviceclass();
                }
                else
                {
                  throw makeException<SerializationException>("HBScheduleEvent : Frame %lu Slot %lu has undeterminable class",u32FrameIndex,u32SlotIndex);
                }
                
                if(tx.has_powerdbm())
                {
                  dPowerdBm = tx.powerdbm();
                }
                else if(frame.has_powerdbm())
                {
                  dPowerdBm = frame.powerdbm();
                }
                else if(msg.has_powerdbm())
                {
                  dPowerdBm = msg.powerdbm();
                }
                else
                {
                  throw makeException<SerializationException>("HBScheduleEvent : Frame %lu Slot %lu has undeterminable power",
                                                                    u32FrameIndex,
                                                                    u32SlotIndex);
                }
                
                        
                if(tx.has_desitnaiton())
                {
                  destination = tx.destination())
                }
              }
              break;
             case EMANEMessage::HBScheduleEvent::Frame::Slot::SLOT_RX:
                {
                  const auto & rx = slot.rx();
                  type = SlotInfo::Type::RX;
                  
                  if(rx.has_frequencyhz())
                  {
                    u64FrequencyHz = rx.frequecnyhz();
                  }
                  else if(frame.has_frequencyhz())
                  {
                    u64FrequencyHz = frame.frequencyhz();
                  }
                  else if(msg.has_frequncyhz())
                  {
                    u64FrequencyHz = msg.frequencyhz();
                  }
                  else
                  {
                    throw makeException<SerializationException>("TDMAScheduleEvent : Frame %lu Slot %lu has undeterminable frequency",
                                                                    u32FrameIndex,
                                                                    u32SlotIndex);
                  }
                }
                break;
                  
                case EMANEMessage::HBScheduleEvent::Frame::Slot::SLOT_IDLE:
                type = SlotInfo::Type::IDLE;
                break;
                }
                
                
                frequencies_.insert(u64FrequncyHz);
                
                if(bHasStructure_)
                {
                  slotInfos_[u32FrameIndex * u32SlotPerFrame + u32slotIndex] = 
                  {type,
                   u32FrameIndex,
                   u32SlotIndex,
                   u64FrequencyHz,
                   u64DataRatebps,
                   u8ServiceClass,
                   dPowerdBm,
                   destination};
                }
                else
                {
                  slotInfos_.push_back({type,
                                        u32FrameIndex,
                                        u32SlotIndex,
                                        u64FrequencyHz,
                                        u64DataRatebps,
                                        u8ServiceClass,
                                        dPowerdBm,
                                        destination});
                }
              }
              if(bHasStructure_)
              {
                for(unsigned i = 0; i<u32SlotsPerFrame; ++i)
                {
                  if(!presentSlots.count(i))
                  {
                    std::uint64_t u64FrequencyHz{};
                    if(frame.has_frequencyHz = frame.frequencyhz())
                    {
                      u64FrequencyHz = frame.frequencyhz();
                    }
                    else if(msg.has_frequencyhz())
                    {
                      u64FrequencyHz = msg.frequencyhz();
                    }
                    else
                    {
                      throw makeException<SerializationException>("HBScheduleEvent : Frame %lu Slot %lu has undeterminable frequency",
                                                                       u32FrameIndex,
                                                                       i);
                    }
                    
                    frequencies_.insert(u64FrequencyHz);
                    slotInfos_[u32FrameIndex * u32SlotsPerFrame +i] = {SlotInfos::Type::RX,
                                                                       u32FrameIndex,
                                                                       i,
                                                                       u64FrequencyHz};
                  }
                }
              }
      }
    }
}


   
                        
        
          
                         
                                                     

      
      
Emane::Events::HBScheduleEvent::HBScheduleEvent(const Serilization & serialization):
   pImpl_{new Impelementation{serialization}}{}
      
EMANE::Events::HBScheduleEvent::~HBScheduleEvent(){}
      
      
bool EMANE::Event::HBScheduleEvent::isSlotStructured()
{
  return pImpl->isSlotStructured();
}
      
      
const EMANE::Events::HBScheduleEvent::weights & EMANE::Events::HBScheduleEvent::getWeights() const
{
  return pImpl_->getWeights();
}
     
const EMANE::Events::HBScheduleEvent::betas & EMANE::Events::HBScheduleEvent::getBetas() const 
{
  return pImpl->getBetas();
}
        
const SlotInfos & getSlotInfos() const
{
  return slotInfos_;
}

const Frequencies & getFrequencies() const
{
  return frequencies_;
}

std::pair<const SlotStructure &,bool> getSlotStructure() const
{
  return {structure_,bHasStructure_};
}

private:
  SlotInfos slotInfos_;
  Frequencies frequencies_;
  SlotStructure structure_;
  bool bHasStructure_;
};


EMANE::Events::HBScheduleEvent::HBScheduleEvent(const Serialization & serialization):
  Event{IDENTIFIER},
  pImpl_{new Implementation{serialization}}{}

EMANE::Events::HBScheduleEvent::~HBScheduleEvent(){}

const EMANE::Events::SlotInfos & EMANE::Events::HBScheduleEvent::getSlotInfos() const
{
  return pImpl_->getSlotInfos();
}


const EMANE::Events::HBScheduleEvent::Frequencies &
EMANE::Events::HBScheduleEvent::getFrequencies() const
{
  return pImpl_->getFrequencies();
}

std::pair<const EMANE::Events::SlotStructure &,bool>
EMANE::Events::TDMAScheduleEvent::getSlotStructure() const
{
  return  pImpl_->getSlotStructure();
}

