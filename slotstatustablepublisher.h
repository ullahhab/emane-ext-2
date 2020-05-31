#ifndef EMANEMODELSHBSLOTSTATUSTABLEPUBLISHER_HEADER_
#define EMANEMODELSHBSLOTSTATUSTABLEPUBLISHER_HEADER_

#include "emane/statisticregistrar.h"
#include <map>
#include <tuple>

namespace EMANE
{
  namespace Models
  {
    namespace HeavyBall
    {
      /**
       * @class SlotStatusTablePublisher
       *
       * @brief Slot statistic and statistic table status publisher.
       *
       * Slot statistic tables can be used to determine how well the
       * emulator is keeping up with the configured slotting
       * structure.
       */
      class SlotStatusTablePublisher
      {
      public:
        enum class Status
          {TX_GOOD,
           TX_MISSED,
           TX_TOOBIG,
           RX_GOOD,
           RX_MISSED,
           RX_IDLE,
           RX_TX,
           RX_TOOLONG,
           RX_WRONGFREQ,
           RX_LOCK};

        void registerStatistics(StatisticRegistrar & registrar);

        void update(std::uint32_t u32RelativeIndex,
                    std::uint32_t u32RelativeFrameIndex,
                    std::uint32_t u32RelativeSlotIndex,
                    Status status,
                    double dSlotRemainingRatio);

        void clear();

      private:
        StatisticTable<std::uint32_t> * pTxSlotStatusTable_;
        StatisticTable<std::uint32_t> * pRxSlotStatusTable_;

        StatisticNumeric<std::uint64_t> * pTxSlotValid_;
        StatisticNumeric<std::uint64_t> * pTxSlotErrorMissed_;
        StatisticNumeric<std::uint64_t> * pTxSlotErrorTooBig_;

        StatisticNumeric<std::uint64_t> * pRxSlotValid_;
        StatisticNumeric<std::uint64_t> * pRxSlotErrorMissed_;
        StatisticNumeric<std::uint64_t> * pRxSlotErrorRxDuringIdle_;
        StatisticNumeric<std::uint64_t> * pRxSlotErrorRxDuringTx_;
        StatisticNumeric<std::uint64_t> * pRxSlotErrorRxTooLong_;
        StatisticNumeric<std::uint64_t> * pRxSlotErrorRxWrongFrequency_;
        StatisticNumeric<std::uint64_t> * pRxSlotErrorRxLock_;

        using TxSlotCounterMap = std::map<std::uint32_t,
                                          std::tuple<std::uint64_t, // valid
                                                     std::uint64_t, // missed
                                                     std::uint64_t, // too big for slot
                                                     std::array<std::uint64_t,8> // quantile
                                                     >>;

        using RxSlotCounterMap = std::map<std::uint32_t,
                                          std::tuple<std::uint64_t, // valid
                                                     std::uint64_t, // missed
                                                     std::uint64_t, // rx during idle
                                                     std::uint64_t, // rx during tx
                                                     std::uint64_t, // rx too long for slot
                                                     std::uint64_t, // rx wrong freq for slot
                                                     std::uint64_t, // rx lock
                                                     std::array<std::uint64_t,8>>>;
        TxSlotCounterMap txSlotCounterMap_;
        RxSlotCounterMap rxSlotCounterMap_;

        void updateRx(std::uint32_t u32RelativeIndex,
                      std::uint32_t u32RelativeFrameIndex,
                      std::uint32_t u32RelativeSlotIndex,
                      Status status,
                      double dSlotPortionRatio);

        void updateTx(std::uint32_t u32RelativeIndex,
                      std::uint32_t u32RelativeFrameIndex,
                      std::uint32_t u32RelativeSlotIndex,
                      Status status,
                      double dSlotPortionRatio);

      };
    }
  }
}

#endif
