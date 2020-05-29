#ifdef EMANEHBBASEMODEL_HEADER_
#define EMANEHBBASEMODEL_HEADER_

#include "heavyballshimLayerImpl.h"
#include "scheduler.h"
#include "HBqueuemanager.h"

namespace EMANE
{
  namespace Models
  {
    namespace HeavyBall
    {
      class HBmodel : public HeavyBallShimLayerImplementor,
                        public SchedulerUser
      {
      public:
        HBmodel(NEMId id,
                  PlatformServiceProvider *platformSeviceProvider,
                  RadioServiceProvider * pRadioServiceProvider,
                  Scheduler * pScheduler,
                  QueueManager * pQueueManager);
                  
       ~HBmodel();
       
       void initialize(Registrar & registrar) overriede;
       
       void configure(const configurationUpdate & update) override;
       
       void start() override;
       
       void postStart() override;
       
       void stop() override;
       
       void destroy() throw() override;
       
       void processUpstreamControl(const ControlMessage & msgs) override;
       
       void processUpstreamPacket(const CommonMACHeader & header,
                                  UpstreamPacket(const CommonMACHeader & pkt,
                                  const ControlMessages & msgs) override;
                                  
       void processDownstreamControl(const ControlMessages & msgs) override;
       
       void processDownstreamPacket(DownstreamPacket & pkt,
                                    const ControlMessages & msgs) override;
                                    
       void processEvent(const EnvetId &, const Serialization &) override;
       
       //TODO: May be we don't need configurations? we can keep it for now.
       void processConfiguration(const ConfigurationUpdate & update) override;
       //TODO: DO we need this line? we don't care about schedule change do we? It doesn't effect anything with scheduler does it?
       void notifyScheduleChange(const Frequencies & frequencies,
                                  std::uint64_t u64BandwidthHz,
                                  const Microseconds & slotDuration,
                                  const Microseconds & slotOverhead,
                                  float beta) override;

       void processSchedulerPacket(DownstreamPacket & pkt) override;
       
       void processSchedulerControl(const ControlMessages & msgs) override;
       
       QueueInfos getPacketQueueInfo() const override;
       

       
       private:
       class Implementation;
       std::unique_ptr<Implemenation> pImpl_;
       };
     }
   }
}    

#endif
