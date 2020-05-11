#ifndef EMANEMODELSTDMAQUEUEMANAGER_HEADER_
#define EMANEMODELSTDMAQUEUEMANAGER_HEADER_

#include "emane/component.h"
#include "emane/platformserviceuser.h"
#include "emane/runningstatemutable.h"
#include "emane/downstreampacket.h"
#include "emane/models/tdma/types.h"
#include "emane/models/tdma/messagecomponent.h"
#include "emane/models/tdma/packetstatuspublisheruser.h"

#include <tuple>
#include <map>

namespace EMANE
{
  namespace Models
  {
    namespace TDMA
    {
      /**
       * @class QueueManager
       *
       * @brief %Queue management interface used by BaseModel
       */
      class QueueManager : public Component,
                           public PlatformServiceUser,
                           public RunningStateMutable,
                           public PacketStatusPublisherUser
      {
      public:
         /**
         * Destroys an instance.
         */
        virtual ~QueueManager(){};

        /**
         * Enqueues a packet in to a specified queue.
         *
         * @param u8QueueIndex %Queue index of target queue
         * @param pkt Packet to enqueue
         *
         * @return The number of packets dropped as a result of this
         * enqueue. This value would be non-zero in the case of
         * overflow.
         */
        virtual
        size_t enqueue(std::uint8_t u8QueueIndex, DownstreamPacket && pkt) = 0;


        /**
         * Dequeues a specified number of bytes from a specified queue.
         *
         * @param u8QueueIndex %Queue index of target queue @param
         * length length to dequeue in bytes
         * @param destination NEM destination to dequeue, 0 indicates
         * any NEM.
         *
         * @return A tuple containing a list of MessageComponent
         * entries and the the total size in bytes across all entries.
         *
         * @note Unless restricted by configuration, the dequeue
         * operation will search multiple queues (highest priority
         * first) in order to return the specified number of bytes. If
         * fragmentation is disabled, all packets in the initially
         * specified queue that are larger than the requested size
         * will be discarded.
         */
        virtual std::tuple<EMANE::Models::TDMA::MessageComponents,
                           size_t>
        dequeue(std::uint8_t u8QueueIndex,
                size_t length,
                NEMId destination) = 0;

        /**
         * Get all destQueue length based on given priority.
         * 
         */
        virtual std::map<std::uint64_t,size_t> getDestQueueLength(int priority) = 0;

        
        /**
         * Gets queue status.
         *
         * @return A list of QueueInfo entries.
         */
        virtual QueueInfos getPacketQueueInfo() const = 0;

      protected:
        NEMId id_;

        /**
         * Creates an instance.
         */
        QueueManager(NEMId id,
                     PlatformServiceProvider * pPlatformServiceProvider):
          PlatformServiceUser{pPlatformServiceProvider},
          id_{id}{}

      private:
        void processEvent(const EventId &, const Serialization &) final{};

        void processTimedEvent(TimerEventId,
                               const TimePoint &,
                               const TimePoint &,
                               const TimePoint &,
                               const void *) final {};

      };
    }
  }
}

#endif
