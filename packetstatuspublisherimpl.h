//TODO: THERE MIGHT BE SOME ISSUES WITH THIS FILE CHECK FOR DEBUGGING!!!!!!



#include "emane/statisticregistrar.h"
#include "packetstatuspublisher.h"

#include <array>
 #include <mutex>
 #include <map>
 
 namespace EMANE
 {
   namespace Models
   {
     namespace heavyball
     {
       class PacketStatusPublisherImpl : public PacketStatusPublisher
       {
       public:
         PacketStatusPublisherImpl();
 
         ~PacketStatusPublisherImpl();
 
         void registerStatistics(StatisticRegistrar & registrar);
 
         void inbound(NEMId src,
                      const MessageComponent & component,
                      InboundAction action) override;
 
         void inbound(NEMId src,
                      const MessageComponents & components,
                      InboundAction action) override;
 
         void inbound(NEMId src,
                      NEMId dst,
                      Priority priority,
                      size_t size,
                      InboundAction action) override;
 
         void outbound(NEMId src,
                       NEMId dst,
                       Priority priority,
                       size_t size,
                       OutboundAction action) override;
 
         void outbound(NEMId src,
                       const MessageComponents & components,
                       OutboundAction action) override;
 
       private:
         enum {QUEUE_COUNT = 5};
 
         using PacketAcceptInfo = std::map<NEMId,std::tuple<std::uint64_t, // bytes Tx
                                                            std::uint64_t>>; // bytes Rx
 
         using PacketDropInfo = std::map<NEMId,std::tuple<std::uint64_t, // SINR
                                                          std::uint64_t, // Reg Id
                                                          std::uint64_t, // Dst MAC
                                                          std::uint64_t, // Queue Overflow
                                                          std::uint64_t, // Bad Control
                                                          std::uint64_t, // Bad Spectrum Query
                                                          std::uint64_t, // Flow Control
                                                          std::uint64_t, // Too Big
                                                          std::uint64_t, // Too Long
                                                          std::uint64_t, // Frequency
                                                          std::uint64_t, // Slot Error
                                                          std::uint64_t>>; // Miss Fragment
 
         using TableArray = std::array<StatisticTable<NEMId> *,QUEUE_COUNT>;
 
         TableArray broadcastAcceptTables_;
         TableArray broadcastDropTables_;
 
         TableArray unicastAcceptTables_;
         TableArray unicastDropTables_;
 
         using AcceptInfoArrary = std::array<PacketAcceptInfo,QUEUE_COUNT>;
         using DropInfoArrary = std::array<PacketDropInfo,QUEUE_COUNT>;
 
         AcceptInfoArrary broadcastAcceptInfos_;
         DropInfoArrary  broadcastDropInfos_;
 
         AcceptInfoArrary unicastAcceptInfos_;
         DropInfoArrary  unicastDropInfos_;
 
         std::mutex mutexBroadcastPacketAcceptTable_;
         std::mutex mutexUnicastPacketAcceptTable_;
         std::mutex mutexBroadcastPacketDropTable_;
         std::mutex mutexUnicastPacketDropTable_;
       };
     }
   }
 }
 
 #endif
