#ifndef EMANEHBTYPE_HEADER_
#define EMANEHBTYPE_HEADER_

#include "emane/types.h"

#include <vector>
#include <set>
#include <list>


namespace EMANE
{
  namespace Models
  {
    namespace HeavyBall
    {
    using SlotIndex = uint64_t;
    using FrameIndex = uint64_t;
    //TODO::RESEARCH_POINT: Maybe you need new packet data structure 
    struct PacketMetaInfo
    {
      NEMId source_;
      SlotIndex slot_;
      double dRxPowerdBm_;
      double dSINR_;
      std::uint64_ u64DataRatebps_;
    };
    
    struct
     
     
     
     
     
     
     }
   }
}
