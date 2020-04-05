#include "messagecomponent.h"
 
 namespace EMANE
 {
   namespace Models
   {
     namespace HeavyBall
     {
       class PacketStatusPublisher
       {
       public:
         virtual ~PacketStatusPublisher(){}
 
         enum class InboundAction
         {
           ACCEPT_GOOD, 
           DROP_BAD_CONTROL,
           DROP_SLOT_NOT_RX,
           DROP_SLOT_MISSED_RX,
           DROP_MISS_FRAGMENT,
           DROP_SPECTRUM_SERVICE,
           DROP_SINR,
           DROP_REGISTRATION_ID,
           DROP_DESTINATION_MAC,
           DROP_TOO_LONG,
           DROP_FREQUENCY
         };
 
         enum class OutboundAction
         {
           ACCEPT_GOOD, 
           DROP_TOO_BIG, 
           DROP_OVERFLOW, 
           DROP_FLOW_CONTROL 
          };
 
         virtual void inbound(NEMId src,
                              const MessageComponent & component,
                              InboundAction action) = 0;
 
         virtual void inbound(NEMId src,
                              const MessageComponents & components,
                              InboundAction action) = 0;
 
         virtual void inbound(NEMId src,
                              NEMId dst,
                              Weight weight,
                              size_t size,
                              InboundAction action) = 0;
 
         virtual void outbound(NEMId src,
                               NEMId dst,
                               weight weight,
                               size_t size,
                               OutboundAction action) = 0;
 
         virtual void outbound(NEMId src,
                               const MessageComponents & components,
                               OutboundAction action) = 0;
 
       protected:
         PacketStatusPublisher(){}
       };
     }
   }
 }
