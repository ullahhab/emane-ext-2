#ifndef EMANEHBSCHEDULER_HEADER_
#define EMANEHBSCHEDULER_HEADER_

#include "emane/component.h"
#include "emane/platformserviceuser.h"
#include "emane/runningstatemutable.h"
#include "emane/upstreampacket.h"

#include "models/heavyball/type.h"
//TODO: LINE MIGHT CHANGE!!!
#include "emane/models/tdma/scheduleruser.h"



namespace EMANE
{
  namespace Models
  {
    namespace HeavyBall
    {
      class Scheduler : public Component,
                        public PlatformServiceUser,
                        public RunningStateMutable
                        
       {
       public:
       
       //TODO: MAYBE SCHeduler need to know who needs to go first or would that be multihop part!! Comes after implementation.
       
       
        virtual ~Scheduler(){};
        virtual void processSchedulerPacket(UpstreamPacket & pkt,
                                            const PacketMetaInfo & packetMetaInfo) = 0;
        virtual voidprocessPacketMetaInfo*const PacketMetaInfo & packetMetaInfo) = 0;
        
        protected:
          SchedulerUser * pSchedulerUser_;
          NEMId id_;
          
          Scheduler(NEMId id, PlatformServiceProvider * pPlatformServiceProvider, SchedulerUser * pSchedulerUser);
          PlatformServiceUser{pPlatformServiceProvider},
          pSchedulerUser_{pSchedulerUser},
          id_{id}{}
          };
    }
  }
}

           
