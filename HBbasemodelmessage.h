#ifndef EMANEMODELHBBASEMODELMESSAGE_HEADER_
#define EMANEMODELHBBASEMODELMESSAGE_HEADER_

#include "emane/serializable.h"
#include "emane/types.h"
#include "messagecomponent.h"

namespace EMANE
{
  namespace Models
  {
    namespace HeavyBall
    {
      class BaseModelMessage : public Serializable
      {
        public:
          BaseModelMessage();
          
          BaseModelMessage(MessageComponents && messages); //TODO::What do we need for the basemodelmessage? channel and queueLength
          
          BaseModelMessage(const void *p, size_t len);
          
          
          const MessageComponents & getMessages() const;
          
          //TODO:NEED MORE FUNCTIONS TO GET QUEUE LENGTH AND OTHER OBJECTS.
          
          Serialization serialize() const override;
          
          private:
            //TODO: NEED MORE FUNCTION to get queue length and other objects
            
            MessageComponents messages_;
            
            };
         }
     }
}
#include "basemodelmessage.inl"
#endif
