#ifndef EMANEEVENTSHBSLOTSTRUCTURE_HEADER_
#define EMANEEVENTSHBSLOTSTRUCTURE_HEADER_

#include "emane/types.h"
#include <set>

namespace EMANE
{
  namespace Events
  {
    class HBSlotStructure
    {
    public:
      SlotStructure(std::uint64_t u64BandwidthHz,
                    std::uint32_t u32FramePerMultiFrame,
                    std::uint32_t u32SlotsPerFrame,
                    const Microseconds & slotDuration,
                    const Microseconds & slotOverhead
                    float Beta);

      SlotStructure();

      std::uint64_t getBandwidth() const;

      std::uint32_t getFramesPerMultiFrame() const;

      std::uint32_t getSlotsPerFrame() const;

      const Microseconds & getSlotDuration() const;

      const Microseconds & getSlotOverhead() const;
      
      float Beta & getBeta() const;

    private:
      std::uint64_t u64BandwidthHz_;
      std::uint32_t u32FramePerMultiFrame_;
      std::uint32_t u32SlotsPerFrame_;
      Microseconds slotDuration_;
      Microseconds slotOverhead_;

    };
  }
}

#include "emane/events/slotstructure.inl"

#endif
