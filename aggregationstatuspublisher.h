#ifndef EMANEMODELSHBAGGREGATIONSTATUSTABLEPUBLISHER_HEADER_
#define EMANEMODELSHBAGGREGATIONSTATUSTABLEPUBLISHER_HEADER_

#include "emane/statisticregistrar.h"
#include "emane/models/tdma/messagecomponent.h"

#include <map>

namespace EMANE
{
  namespace Models
  {
    namespace HeavyBall
    {
      /**
       * @class AggregationStatusPublisher
       *
       * @brief Aggregation statistic and statistic table status publisher.
       */
      class AggregationStatusPublisher
      {
      public:
        AggregationStatusPublisher();

        void registerStatistics(StatisticRegistrar & registrar);

        void update(const MessageComponents & components);

      private:
        StatisticTable<uint64_t> * pAggregationHistogramTable_;

        using AggregationHistogram = std::map<uint64_t,std::uint64_t>;

        AggregationHistogram aggregationHistogram_;
      };
    }
  }
}

#endif
