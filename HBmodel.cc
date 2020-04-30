#include "emane/models/heavyball/HBmodel.h"
#include "HBmodelimpl.h"


EMANE::Models::HeavyBall::HBmodel::HBmodel(NEMId id,
                                           PlatformServiceProvider * pPlatformServiceProvider,
                                           RadioServiceProvider * pRadioServiceProvider,
                                           Scheduler * pscheduler,
                                           Queuemanager * pQueueManager):
  
  HeavyBallShimLayerImplementor{id, pPlatformServiceProvider, pRadioServiceProvider}
  pImpl_{new Implementation{id,
                            pPlatformServiceProvider,
                            pRadioServiceProvider,
                            pScheduler,
                            pQueueManager,
                            this}}
{}


EMANE::Models::HeavyBall::HBmodel::~HBmodel()
{
}

void 
EMANE::Model::HeavyBall::HBmodel::initialize(Registrar & registrar)
{
  pImpl_->initialize(registrar);
}

void
EMANE::Models::HeavyBall::HBmodel::configure(const ConfigurationUpdate & update)
{
  pImple_->configure(update);
}

EMANE::Models::HeavyBall::HBmodel::start()
{
  pImpl_->start();
}

EMANE::Models::HeavyBall::HBmodel::postStart()
{
  pImpl_->postStart();
}


EMANE::Models::HeavyBall::HBmodel::stop()
{
  pImpl_->stop();
}


EMANE::Models::HeavyBall::HBmodel::destroy()
{
  pImpl_->destroy();
}


EMANE::Models::HeavyBall::HBmodel::processUpstreamControl(const ControlMessages & msgs)
{
  pImpl_->processUpstreamControl(msgs);
}


EMANE::Models::HeavyBall::HBmodel::processUpstreamPacket(const CommonMACHeader & hdr,
                                                          UpstreamPacket & pkt,
                                                          const ControlMessages & msgs)
{
  pImpl_->processUpstreamPacket(hdr,pkt,msgs);
}

EMANE::Models::HeavyBall::HBmodel::processDownStreamPacket(DownstreamPacket & pkt,
                                                           const ControlMessages & msgs)
{
  pImpl_->processDownstreamPacket(msgs);
}


EMANE::Models::HeavyBall::HBmodel::processDownStreamConstrol(const ControlMessages & msgs)
{
  pImpl_->processDownstreamConstrol(msgs);
}

EMANE::Models::HeavyBall::HBmodel::processEvent(const EventId & eventId,
                                                const Serialization & serialization)
{
  pImpl_->processEvent(eventId,serializtion);
}

          
EMANE::Models::HeavyBall::HBmodel::processConfiguration(const ConfigurationUpdate & update)
{
  pImpl_->processConfiguration(update);
}

EMANE::Models::HeavyBall::HBmodel::notifyScheduleChange(const Frequencies & frequencies,
                                                        std::uint64_t u64BandwidthHz,
                                                        const Microseconds & slotDuration,
                                                        const Microseconds & slotOverhead)
{
  pImpl_->notifyScheduleChange(frequencies, u64BandwidthHz, slotDuration, slotOverhead);
}


EMANE::Models::HeavyBall::HBmodel::processSchedulerPacket(DownstreamPacket & pkt)
{
  pImpl_->processSchedulerPacket(pkt);
}


EMANE::Models::HeavyBall::HBmodel::processSchedulerControl(const ControlMessages & msgs)
{
  pImpl_->processSchedulerControl(msgs);
}

EMANE::Models::TDMA::QueueInfos EMANE::Models::HeavyBall::HBmodel::getPacketQueueInfo() const
{
  return pImpl_->getPacketQueueInfo();
}
  
