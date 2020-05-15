 #ifndef EMANEHBSCHEDULERUSER_HEADER_
 #define EMANEHBSCHEDULERUSER_HEADER_
 
 #include "emane/models/tdma/types.h"
 #include "emane/downstreampacket.h"
 #include "emane/controlmessage.h"
 #include "HeavyBall.h"
 
 namespace EMANE
 {
   namespace Models
   {
     namespace HeavyBall
     {
       class SchedulerUser
       {
       public:
         virtual ~SchedulerUser(){};
 
         virtual void notifyScheduleChange(const Frequencies & frequencies,
                                           std::uint64_t u64BandwidthHz,
                                           const Microseconds & slotDuration,
                                           const Microseconds & slotOverhead
                                           float beta) = 0;
 
         virtual void processSchedulerPacket(DownstreamPacket & pkt) = 0;
 
         virtual void processSchedulerControl(const ControlMessages & msgs) = 0;
 
 
         virtual QueueInfos getPacketQueueInfo() const = 0;
 
 
         static const ControlMessages empty;
 
       protected:
         SchedulerUser(){}
       };
     }
   }
 }
 
 #endif
