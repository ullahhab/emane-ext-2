#ifdef EMANEHBBASEMODEL_HEADER_
#define EMANEHBBASEMODEL_HEADER_

#include "heavyballshimLayerImpl.h"
#include "emane/models/HeavyBall/scheduler.h"
#inlcude "emane/models/tdma/queuemanager.h"

namespace EMANE
{
  namespace Models
  {
    namespace HeavyBall
    {
      class BaseModel : public HeavyBallShimLayerImplementor,
                        public SchedulerUser
      {
      public:
        BaseModel(NEMId id,
                  PlatformServiceProvider *platformSeviceProvider,
                  RadioServiceProvider * pRadioServiceProvider,
                  Scheduler * pScheduler,
                  QueueManager * pQueueManager);
                  
       BaseModel();
       
       void initialize(Registrar & registrar) overriede;
       
       void configure(const configurationUpdate & update) override;
       
       void start() override;
       
       void postStart() override;
       
       void stop() override;
       
       void destroy() override;
       
       void processUpstreamControl(const ControlMessage & msgs) override;
       
       void processUpstreamPacket(const CommonMACHeader & header,
                                  UpstreamPacket(const CommonMACHeader & pkt,
                                  const ControlMessages & msgs) override;
                                  
       void processDownstreamControl(const ControlMessages & msgs) override;
       
       void processDownstreamPacket(DownstreamPacket & pkt,
                                    const ControlMessages & msgs) override;
                                    
       void processEvent(const EnvetId &, const Serialization &) override;
       
       void processConfiguration(const ConfigurationUpdate & update) override;
       //TODO: DO we need this line? we don't care about schedule change do we? It doesn't effect anything with scheduler does it?
       //void notifyScheduleChange(const Frequencies & frequencies,std::unint64_t u64BandwidthHz,const Micorseconds & slotDuration, const micro
