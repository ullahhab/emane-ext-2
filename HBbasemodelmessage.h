#ifndef EMANEMODELHBBASEMODELMESSAGE_HEADER_
#define EMANEMODELHBBASEMODELMESSAGE_HEADER_

#include "emane/serializable.h"
#include "emane/types.h"
#include "emane/models/tdma/messagecomponent.h"

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

        BaseModelMessage(std::uint64_t u64AbsoluteSlotIndex,
                         std::uint64_t u64DataRatebps,
                         MessageComponents && messages);

        BaseModelMessage(const void * p, size_t len);

        const MessageComponents & getMessages() const;

        std::uint64_t getAbsoluteSlotIndex() const;

        std::uint64_t getDataRate() const;

        Serialization serialize() const override;

      private:
        std::uint64_t u64AbsoluteSlotIndex_;
        std::uint64_t u64DataRatebps_;
        MessageComponents messages_;
      };
    }
  }
}
#include "HBbasemodelmessage.inl"
#endif
