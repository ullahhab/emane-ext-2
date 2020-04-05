#ifndef EMANEHBSCHEDULER_HEADER_
define EMANEHBSCHEDULER_HEADER_

#include "scheduler.h"
//TODO::RESEARCH POINT: NEED MORE ITEM TO SCHEDULE USER.



namespace EMANE
{
  namespace Models
  {
    namespace heavyball
    {
      class EventScheduler : public Scheduler
      {
       public:
        EventScheduler(NEMId id) //TODO::RESEARCH POINT: VALUES NEEDED such as maxweight and all other. Question for reserch can I trasmit my message to TDMA? Will it work with it or crash the RadioModel?
        
        ~EventScheduler();
        
        void initialize(Registrar & registrar) override;

        void configure(const ConfigurationUpdate & update) override;

        void start() override;

        void postStart() override;

        void stop() override;

        void destroy() throw() override;

        void processEvent(const EventId & eventId,
                          const Serialization & serialization) override;
                          
        void processSchedulerPacket(UpstreamPacket & pkt,
                                    const PacketMetaInfo & packetMetaInfo) override;

        void processPacketMetaInfo(const PacketMetaInfo & packetMetaInfo) override;
        
        
        private:
        //TODO: Values would go here AGAIN NOT SURE WHAT NEED TO RESEARCH
        
        
        //TODO: FLUSHSCHEDULE???   src/models/mac/tdma/eventscheduler/eventscheduler.h
        
        
        
        
      };
    }
  }
}

#endif //EMANEHBSCHEDULER_HEADER_
      
      
