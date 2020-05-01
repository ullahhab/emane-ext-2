#include "HBmodelimpl.h"
#include "basemodelimpl.h"
#include "emane/models/tdma/queuemanager.h"

#include "emane/configureexception.h"
#include "emane/controls/frequencyofinterestcontrolmessage.h"
#include "emane/controls/flowcontrolcontrolmessage.h"
#include "emane/controls/serializedcontrolmessage.h"
#include "emane/mactypes.h"

#include "emane/controls/frequencycontrolmessage.h"
#include "emane/controls/frequencycontrolmessageformatter.h"
#include "emane/controls/receivepropertiescontrolmessage.h"
#include "emane/controls/receivepropertiescontrolmessageformatter.h"
#include "emane/controls/timestampcontrolmessage.h"
#include "emane/controls/transmittercontrolmessage.h"

#include "emane/models/tdma/txslotinfosformatter.h"
#include "HBbasemodelmessage.h"
#include "emane/models.tdma/priority.h"



namespace
{
  const std::string QUEUMANAGER_PREFIX{"queue."};
  const std::string SCHEDULER_PREFIX{"scheduler."};
}


EMANE::Models::HeavyBall::HBmodel::Implementation::
Implementation(NEMId id,
               PlatformServiceProvider *pPlatformServiceProvider,
               RadioServiceProvider * pRadioServiceProvider,
               Scheduler * pScheduler,
               QueueManager * pQueueManager,
               MACLayerImplementor * pRadioModel):
  MACLayerImplementor{id,pPlatformServiceProvider,pRadioServiceProvider},
  pScheduler_{pScheduler},
  pQueueManager_{pQueueManager},
  pRadioModel_{pRadioModel},
  bFlowControlEnable_{},
  u16FlowControlTokens_{},
  sPCRCurveURI_{},
  transmitTimedEventId_{},
  nextMultiFrameTime_{},
  txSlotInfos_{},
  slotDuration_{},
  slotOverhead_{},
  u64SequenceNumber_{},
  frequencies_{},
  u64BandwidthHz_{},
  packetStatusPublisher_{},
  neighborMetricManager_{id},
  receiveManager_{id,
      pRadioModel,
      &pPlatformServiceProvider->logService(),
      pRadioServiceProvider,
      pScheduler,
      &packetStatusPublisher_,
      &neighborMetricManager_},
  flowControlManager_{*pRadioModel},
  u64ScheduleIndex_{}{}


EMANE::Models::HeavyBall::HBmodel::Implementation::~Implementation()
{}


void 
EMANE::Models::HeavyBall::HBmodel::Implementation::initialize(Registrar & registrar)
{
  LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                          DEBUG_LEVEL,
                          "MACI %03hu HeavyBall::HBmodel::%s",
                          id_,
                          __func__);

  auto & configRegistrar = registrar.configurationRegistrar();

  configRegistrar.registerNumeric<bool>("enablepromiscuousmode",
                                        ConfigurationProperties::DEFAULT |
                                        ConfigurationProperties::MODIFIABLE,
                                        {false},
                                        "Defines whether promiscuous mode is enabled or not."
                                        " If promiscuous mode is enabled, all received packets"
                                        " (intended for the given node or not) that pass the"
                                        " probability of reception check are sent upstream to"
                                        " the transport.");


  configRegistrar.registerNumeric<bool>("flowcontrolenable",
                                        ConfigurationProperties::DEFAULT,
                                        {false},
                                        "Defines whether flow control is enabled. Flow control only works"
                                        " with the virtual transport and the setting must match the setting"
                                        " within the virtual transport configuration.");

  configRegistrar.registerNumeric<std::uint16_t>("flowcontroltokens",
                                                 ConfigurationProperties::DEFAULT,
                                                 {10},
                                                 "Defines the maximum number of flow control tokens"
                                                 " (packet transmission units) that can be processed from the"
                                                 " virtual transport without being refreshed. The number of"
                                                 " available tokens at any given time is coordinated with the"
                                                 " virtual transport and when the token count reaches zero, no"
                                                 " further packets are transmitted causing application socket"
                                                 " queues to backup.");

  configRegistrar.registerNonNumeric<std::string>("pcrcurveuri",
                                                  ConfigurationProperties::REQUIRED,
                                                  {},
                                                  "Defines the URI of the Packet Completion Rate (PCR) curve"
                                                  " file. The PCR curve file contains probability of reception curves"
                                                  " as a function of Signal to Interference plus Noise Ratio (SINR).");


  configRegistrar.registerNumeric<std::uint16_t>("fragmentcheckthreshold",
                                                 ConfigurationProperties::DEFAULT,
                                                 {2},
                                                 "Defines the rate in seconds a check is performed to see if any packet"
                                                 " fragment reassembly efforts should be abandoned.");

  configRegistrar.registerNumeric<std::uint16_t>("fragmenttimeoutthreshold",
                                                 ConfigurationProperties::DEFAULT,
                                                 {5},
                                                 "Defines the threshold in seconds to wait for another packet fragment"
                                                 " for an existing reassembly effort before abandoning the effort.");

  configRegistrar.registerNumeric<float>("neighbormetricdeletetime",
                                         ConfigurationProperties::DEFAULT |
                                         ConfigurationProperties::MODIFIABLE,
                                         {60.0f},
                                         "Defines the time in seconds of no RF receptions from a given neighbor"
                                         " before it is removed from the neighbor table.",
                                         1.0f,
                                         3660.0f);


  configRegistrar.registerNumeric<float>("neighbormetricupdateinterval",
                                         ConfigurationProperties::DEFAULT,
                                         {1.0f},
                                         "Defines the neighbor table update interval in seconds.",
                                         0.1f,
                                         60.0f);

  auto & statisticRegistrar = registrar.statisticRegistrar();

  packetStatusPublisher_.registerStatistics(statisticRegistrar);

  slotStatusTablePublisher_.registerStatistics(statisticRegistrar);

  neighborMetricManager_.registerStatistics(statisticRegistrar);

  aggregationStatusPublisher_.registerStatistics(statisticRegistrar);

  pQueueManager_->setPacketStatusPublisher(&packetStatusPublisher_);

  pQueueManager_->initialize(registrar);

  pScheduler_->initialize(registrar);
}


void
EMANE::Models::HeavyBall::HBmodel::Implementation::configuration(const ConfigurationUpdate & update)
{
  LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                          DEBUG_LEVEL,
                          "MACI %03hu HeavyBall::HBmodel::%s",
                          id_,
                          __func__);

  ConfigurationUpdate schedulerConfiguration{};
  ConfigurationUpdate queueManagerConfiguration{};

  for(const auto & item : update)
    {
      if(item.first == "enablepromiscuousmode")
        {
          bool bPromiscuousMode{item.second[0].asBool()};

          LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                  INFO_LEVEL,
                                  "MACI %03hu HeavyBall::HBmodel::%s: %s = %s",
                                  id_,
                                  __func__,
                                  item.first.c_str(),
                                  bPromiscuousMode ? "on" : "off");

          receiveManager_.setPromiscuousMode(bPromiscuousMode);
        }
      else if(item.first == "flowcontrolenable")
        {
          bFlowControlEnable_ = item.second[0].asBool();

          LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                  INFO_LEVEL,
                                  "MACI %03hu HeavyBall::HBmodel::%s: %s = %s",
                                  id_,
                                  __func__,
                                  item.first.c_str(),
                                  bFlowControlEnable_ ? "on" : "off");
        }
      else if(item.first == "flowcontroltokens")
        {
          u16FlowControlTokens_ = item.second[0].asUINT16();

          LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                  INFO_LEVEL,
                                  "MACI %03hu HeavyBall::HBmodel::%s: %s = %hu",
                                  id_,
                                  __func__,
                                  item.first.c_str(),
                                  u16FlowControlTokens_);
        }
      else if(item.first == "pcrcurveuri")
        {
          sPCRCurveURI_ = item.second[0].asString();

          LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                  INFO_LEVEL,
                                  "MACI %03hu HeavyBall::HBmodel::%s: %s = %s",
                                  id_,
                                  __func__,
                                  item.first.c_str(),
                                  sPCRCurveURI_.c_str());

          receiveManager_.loadCurves(sPCRCurveURI_);
        }
      else if(item.first == "fragmentcheckthreshold")
        {
          std::chrono::seconds fragmentCheckThreshold{item.second[0].asUINT16()};

          LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                  INFO_LEVEL,
                                  "MACI %03hu HeavyBall::HBmodel::%s: %s = %lu",
                                  id_,
                                  __func__,
                                  item.first.c_str(),
                                  fragmentCheckThreshold.count());

          receiveManager_.setFragmentCheckThreshold(fragmentCheckThreshold);
        }
      else if(item.first == "fragmenttimeoutthreshold")
        {
          std::chrono::seconds fragmentTimeoutThreshold{item.second[0].asUINT16()};

          LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                  INFO_LEVEL,
                                  "MACI %03hu HeavyBall::HBmodel::%s: %s = %lu",
                                  id_,
                                  __func__,
                                  item.first.c_str(),
                                  fragmentTimeoutThreshold.count());

          receiveManager_.setFragmentTimeoutThreshold(fragmentTimeoutThreshold);
        }
      else if(item.first == "neighbormetricdeletetime")
        {
          Microseconds neighborMetricDeleteTimeMicroseconds =
            std::chrono::duration_cast<Microseconds>(DoubleSeconds{item.second[0].asFloat()});

          // set the neighbor delete time
          neighborMetricManager_.setNeighborDeleteTimeMicroseconds(neighborMetricDeleteTimeMicroseconds);

          LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                  DEBUG_LEVEL,
                                  "MACI %03hu HeavyBall::HBmodel::%s %s = %lf",
                                  id_,
                                  __func__,
                                  item.first.c_str(),
                                  std::chrono::duration_cast<DoubleSeconds>(neighborMetricDeleteTimeMicroseconds).count());
        }
      else if(item.first == "neighbormetricupdateinterval")
        {
          neighborMetricUpdateInterval_ =
            std::chrono::duration_cast<Microseconds>(DoubleSeconds{item.second[0].asFloat()});

          LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                  INFO_LEVEL,
                                  "MACI %03hu HeavyBall::HBmodel::%s %s = %lf",
                                  id_,
                                  __func__,
                                  item.first.c_str(),
                                  std::chrono::duration_cast<DoubleSeconds>(neighborMetricUpdateInterval_).count());
        }
      else
        {
          if(!item.first.compare(0,SCHEDULER_PREFIX.size(),SCHEDULER_PREFIX))
            {
              schedulerConfiguration.push_back(item);
            }
          else if(!item.first.compare(0,QUEUEMANAGER_PREFIX.size(),QUEUEMANAGER_PREFIX))
            {
              queueManagerConfiguration.push_back(item);
            }
          else
            {
              throw makeException<ConfigureException>("HeavyBall::HBmodel: "
                                                      "Ambiguous configuration item %s.",
                                                      item.first.c_str());
            }
        }
    }

  pQueueManager_->configure(queueManagerConfiguration);

  pScheduler_->configure(schedulerConfiguration);
}

void 
EMANE::Models::HeavyBall::HBmodel::Implementation::start()
{
  LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                          DEBUG_LEVEL,
                          "MACI %03hu HeavyBall::HBmodel::%s",
                          id_,
                          __func__);

  pQueueManager_->start();

  pScheduler_->start();
}


void
EMANE::Models::HeavyBall::HBmodel::Implementation::postStart()
{
  LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                          DEBUG_LEVEL,
                          "MACI %03hu TDMA::BaseModel::%s",
                          id_,
                          __func__);

  pQueueManager_->postStart();

  pScheduler_->postStart();

  // check flow control enabled
  if(bFlowControlEnable_)
    {
      // start flow control
      flowControlManager_.start(u16FlowControlTokens_);

      LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                              DEBUG_LEVEL,
                              "MACI %03hu HeavyBall::HBmodel::%s sent a flow control token update,"
                              " a handshake response is required to process packets",
                              id_,
                              __func__);
    }

  pPlatformService_->timerService().
    schedule(std::bind(&NeighborMetricManager::updateNeighborStatus,
                       &neighborMetricManager_),
             Clock::now() + neighborMetricUpdateInterval_,
             neighborMetricUpdateInterval_);
}


void
EMANE::Models::HeavyBall::HBmodel::Implementation::stop()
{
  LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                          DEBUG_LEVEL,
                          "MACI %03hu HeavyBall::HBmodel::%s",
                          id_,
                          __func__);

  // check flow control enabled
  if(bFlowControlEnable_)
    {
      // stop the flow control manager
      flowControlManager_.stop();
    }

  pQueueManager_->stop();

  pScheduler_->stop();
}


void
EMANE::Models::HeavyBall:HBmodel::Implementation::destroy()
  throw()
{
  LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                          DEBUG_LEVEL,
                          "MACI %03hu HeavyBall::HBmodel::%s",
                          id_,
                          __func__);

  pQueueManager_->destroy();

  pScheduler_->destroy();
}


void
EMANE::Models::HeavyBall::HBmodel::Implementation::processUpstreamControl(const ControlMessages &)
{
  LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                          DEBUG_LEVEL,
                          "MACI %03hu HeavyBall::HBmodel::%s",
                          id_,
                          __func__);

}


//TODO::Check if it covers all the things we need to demonstrate.

void
EMANE::Models::HeavyBall::HBmodel::Implementation::processUpstreamPacket(const CommonMACHeader & header,
                                                                         UpstreamPacket & pkt,
                                                                         const ControlMessages & msgs)
{
  auto now = Clock::now();

  LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                          DEBUG_LEVEL,
                          "MACI %03hu TDMA::BaseModel::%s",
                          id_,
                          __func__);


  const PacketInfo & pktInfo{pkt.getPacketInfo()};

  if(hdr.getRegistrationId() != REGISTERED_EMANE_MAC_TDMA)
    {
      LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                              ERROR_LEVEL,
                              "MACI %03hu TDMA::BaseModel::%s: MAC Registration Id %hu does not match our Id %hu, drop.",
                              id_,
                              __func__,
                              hdr.getRegistrationId(),
                              REGISTERED_EMANE_MAC_TDMA);


      packetStatusPublisher_.inbound(pktInfo.getSource(),
                                     pktInfo.getSource(),
                                     pktInfo.getPriority(),
                                     pkt.length(),
                                     PacketStatusPublisher::InboundAction::DROP_REGISTRATION_ID);

      // drop
      return;
    }

  size_t len{pkt.stripLengthPrefixFraming()};

  if(len && pkt.length() >= len)
    {
      BaseModelMessage baseModelMessage{pkt.get(), len};


      const Controls::ReceivePropertiesControlMessage * pReceivePropertiesControlMessage{};

      const Controls::FrequencyControlMessage * pFrequencyControlMessage{};

      for(auto & pControlMessage : msgs)
        {
          switch(pControlMessage->getId())
            {
            case EMANE::Controls::ReceivePropertiesControlMessage::IDENTIFIER:
              {
                pReceivePropertiesControlMessage =
                  static_cast<const Controls::ReceivePropertiesControlMessage *>(pControlMessage);

                LOGGER_VERBOSE_LOGGING_FN_VARGS(pPlatformService_->logService(),
                                                DEBUG_LEVEL,
                                                Controls::ReceivePropertiesControlMessageFormatter(pReceivePropertiesControlMessage),
                                                "MACI %03hu TDMA::BaseModel::%s Receiver Properties Control Message",
                                                id_,
                                                __func__);
              }
              break;

            case Controls::FrequencyControlMessage::IDENTIFIER:
              {
                pFrequencyControlMessage =
                  static_cast<const Controls::FrequencyControlMessage *>(pControlMessage);

                LOGGER_VERBOSE_LOGGING_FN_VARGS(pPlatformService_->logService(),
                                                DEBUG_LEVEL,
                                                Controls::FrequencyControlMessageFormatter(pFrequencyControlMessage),
                                                "MACI %03hu TDMA::BaseModel::%s Frequency Control Message",
                                                id_,
                                                __func__);

              }

              break;
            }
        }

      if(!pReceivePropertiesControlMessage || !pFrequencyControlMessage ||
         pFrequencyControlMessage->getFrequencySegments().empty())
        {
          LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                  ERROR_LEVEL,
                                  "MACI %03hu TDMA::BaseModel::%s: phy control "
                                  "message not provided from src %hu, drop",
                                  id_,
                                  __func__,
                                  pktInfo.getSource());

          packetStatusPublisher_.inbound(pktInfo.getSource(),
                                         baseModelMessage.getMessages(),
                                         PacketStatusPublisher::InboundAction::DROP_BAD_CONTROL);

          // drop
          return;
        }

      const auto & frequencySegments = pFrequencyControlMessage->getFrequencySegments();

      const FrequencySegment & frequencySegment{*frequencySegments.begin()};

      TimePoint startOfReception{pReceivePropertiesControlMessage->getTxTime() +
          pReceivePropertiesControlMessage->getPropagationDelay() +
          frequencySegment.getOffset()};


      // if EOR slot does not match the SOT slot drop the packet
      auto eorSlotInfo = pScheduler_->getSlotInfo(startOfReception +
                                                  frequencySegment.getDuration());

      // message is too long for slot
      if(eorSlotInfo.u64AbsoluteSlotIndex_ != baseModelMessage.getAbsoluteSlotIndex())
        {
          // determine current slot based on now time to update rx slot status table
          auto slotInfo = pScheduler_->getSlotInfo(now);

          Microseconds timeRemainingInSlot{};

          // ratio calcualtion for slot status tables
          if(slotInfo.u64AbsoluteSlotIndex_ == baseModelMessage.getAbsoluteSlotIndex())
            {
              timeRemainingInSlot = slotDuration_ -
                std::chrono::duration_cast<Microseconds>(now -
                                                         slotInfo.timePoint_);
            }
          else
            {
              timeRemainingInSlot = slotDuration_ +
                std::chrono::duration_cast<Microseconds>(now -
                                                         slotInfo.timePoint_);
            }

          double dSlotRemainingRatio =
            timeRemainingInSlot.count() / static_cast<double>(slotDuration_.count());

          slotStatusTablePublisher_.update(slotInfo.u32RelativeIndex_,
                                           slotInfo.u32RelativeFrameIndex_,
                                           slotInfo.u32RelativeSlotIndex_,
                                           SlotStatusTablePublisher::Status::RX_TOOLONG,
                                           dSlotRemainingRatio);

          packetStatusPublisher_.inbound(pktInfo.getSource(),
                                         baseModelMessage.getMessages(),
                                         PacketStatusPublisher::InboundAction::DROP_TOO_LONG);


          LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                  DEBUG_LEVEL,
                                  "MACI %03hu TDMA::BaseModel::%s eor rx slot:"
                                  " %zu does not match sot slot: %zu, drop long",
                                  id_,
                                  __func__,
                                  eorSlotInfo.u64AbsoluteSlotIndex_,
                                  baseModelMessage.getAbsoluteSlotIndex());

          // drop
          return;
        }

      // rx slot info for now
      auto entry = pScheduler_->getRxSlotInfo(now);

      if(entry.first.u64AbsoluteSlotIndex_ == baseModelMessage.getAbsoluteSlotIndex())
        {
          Microseconds timeRemainingInSlot{slotDuration_ -
              std::chrono::duration_cast<Microseconds>(now -
                                                       entry.first.timePoint_)};

          double dSlotRemainingRatio =
            timeRemainingInSlot.count() / static_cast<double>(slotDuration_.count());

          if(entry.second)
            {
              if(entry.first.u64FrequencyHz_ == frequencySegment.getFrequencyHz())
                {
                  // we are in an RX Slot
                  slotStatusTablePublisher_.update(entry.first.u32RelativeIndex_,
                                                   entry.first.u32RelativeFrameIndex_,
                                                   entry.first.u32RelativeSlotIndex_,
                                                   SlotStatusTablePublisher::Status::RX_GOOD,
                                                   dSlotRemainingRatio);

                  Microseconds span{pReceivePropertiesControlMessage->getSpan()};

                  if(receiveManager_.enqueue(std::move(baseModelMessage),
                                             pktInfo,
                                             pkt.length(),
                                             startOfReception,
                                             frequencySegments,
                                             span,
                                             now,
                                             hdr.getSequenceNumber()))
                    {
                      pPlatformService_->timerService().
                        schedule(std::bind(&ReceiveManager::process,
                                           &receiveManager_,
                                           entry.first.u64AbsoluteSlotIndex_+1),
                                 entry.first.timePoint_+ slotDuration_);
                    }
                }
              else
                {
                  slotStatusTablePublisher_.update(entry.first.u32RelativeIndex_,
                                                   entry.first.u32RelativeFrameIndex_,
                                                   entry.first.u32RelativeSlotIndex_,
                                                   SlotStatusTablePublisher::Status::RX_WRONGFREQ,
                                                   dSlotRemainingRatio);

                  packetStatusPublisher_.inbound(pktInfo.getSource(),
                                                 baseModelMessage.getMessages(),
                                                 PacketStatusPublisher::InboundAction::DROP_FREQUENCY);

                  LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                          DEBUG_LEVEL,
                                          "MACI %03hu TDMA::BaseModel::%s drop reason rx slot correct"
                                          " rframe: %u rslot: %u but frequency mismatch expected: %zu got: %zu",
                                          id_,
                                          __func__,
                                          entry.first.u32RelativeFrameIndex_,
                                          entry.first.u32RelativeSlotIndex_,
                                          entry.first.u64FrequencyHz_,
                                          frequencySegment.getFrequencyHz());

                  // drop
                  return;
                }
            }
          else
            {
              // not an rx slot but it is the correct abs slot
              auto slotInfo = pScheduler_->getSlotInfo(entry.first.u64AbsoluteSlotIndex_);

              slotStatusTablePublisher_.update(entry.first.u32RelativeIndex_,
                                               entry.first.u32RelativeFrameIndex_,
                                               entry.first.u32RelativeSlotIndex_,
                                               slotInfo.type_ == SlotInfo::Type::IDLE ?
                                               SlotStatusTablePublisher::Status::RX_IDLE :
                                               SlotStatusTablePublisher::Status::RX_TX,
                                               dSlotRemainingRatio);

              packetStatusPublisher_.inbound(pktInfo.getSource(),
                                             baseModelMessage.getMessages(),
                                             PacketStatusPublisher::InboundAction::DROP_SLOT_NOT_RX);


              LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                      DEBUG_LEVEL,
                                      "MACI %03hu TDMA::BaseModel::%s drop reason rx slot correct but %s rframe: %u rslot: %u",
                                      id_,
                                      __func__,
                                      slotInfo.type_ == SlotInfo::Type::IDLE ?
                                      "in idle" : "in tx",
                                      entry.first.u32RelativeFrameIndex_,
                                      entry.first.u32RelativeSlotIndex_);


              // drop
              return;
            }
        }
      else
        {
          auto slotInfo = pScheduler_->getSlotInfo(entry.first.u64AbsoluteSlotIndex_);

          Microseconds timeRemainingInSlot{slotDuration_ +
              std::chrono::duration_cast<Microseconds>(now -
                                                       slotInfo.timePoint_)};
          double dSlotRemainingRatio =
            timeRemainingInSlot.count() / static_cast<double>(slotDuration_.count());


          // were we supposed to be in rx on the pkt abs slot
          if(slotInfo.type_ == SlotInfo::Type::RX)
            {
              slotStatusTablePublisher_.update(entry.first.u32RelativeIndex_,
                                               entry.first.u32RelativeFrameIndex_,
                                               entry.first.u32RelativeSlotIndex_,
                                               SlotStatusTablePublisher::Status::RX_MISSED,
                                               dSlotRemainingRatio);

              packetStatusPublisher_.inbound(pktInfo.getSource(),
                                             baseModelMessage.getMessages(),
                                             PacketStatusPublisher::InboundAction::DROP_SLOT_MISSED_RX);

              LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                      DEBUG_LEVEL,
                                      "MACI %03hu TDMA::BaseModel::%s drop reason slot mismatch pkt: %zu now: %zu ",
                                      id_,
                                      __func__,
                                      baseModelMessage.getAbsoluteSlotIndex(),
                                      entry.first.u64AbsoluteSlotIndex_);

              // drop
              return;
            }
          else
            {
              slotStatusTablePublisher_.update(entry.first.u32RelativeIndex_,
                                               entry.first.u32RelativeFrameIndex_,
                                               entry.first.u32RelativeSlotIndex_,
                                               slotInfo.type_ == SlotInfo::Type::IDLE ?
                                               SlotStatusTablePublisher::Status::RX_IDLE :
                                               SlotStatusTablePublisher::Status::RX_TX,
                                               dSlotRemainingRatio);

              packetStatusPublisher_.inbound(pktInfo.getSource(),
                                             baseModelMessage.getMessages(),
                                             PacketStatusPublisher::InboundAction::DROP_SLOT_NOT_RX);


              LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                                      DEBUG_LEVEL,
                                      "MACI %03hu TDMA::BaseModel::%s drop reason slot mismatch but %s pkt: %zu now: %zu ",
                                      id_,
                                      __func__,
                                      slotInfo.type_ == SlotInfo::Type::IDLE ?
                                      "in idle" : "in tx",
                                      baseModelMessage.getAbsoluteSlotIndex(),
                                      entry.first.u64AbsoluteSlotIndex_);

              // drop
              return;
            }
        }
    }
  else
    {
      LOGGER_STANDARD_LOGGING(pPlatformService_->logService(),
                              ERROR_LEVEL,
                              "MACI %03hu TDMA::BaseModel::%s Packet payload length %zu does not match length prefix %zu",
                              id_,
                              __func__,
                              pkt.length(),
                              len);
    }
}




