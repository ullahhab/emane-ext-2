#include "emane/type.h"
#include "emane/events/HeavyBall.inl"

#inlcude <list>
#ifndef EMANEEVENTSHEAVYBALL_HEADER_
#define EMANEEVENTSHEAVYBALL_HEADER_
namespace EMANE
{
  namespace Events
  {
    class HeavyBall
    {
    public:
      HeavyBall(NEMId nemid,
                std::uint64_t weight,
                float beta,
                const hasSlotStructure)
        
        
        
      NEMId getNEMId() const;
      std::uint64_t getWeight() const;
      float getBeta() const;
      
      
   private:
      NEMId neId_;
      std::uint64_t weight_;
      float beta_;
    };
    
    using HeavyBall = std::list<HeavyBall>
  }
      
}

#endif

      
      
      
                
