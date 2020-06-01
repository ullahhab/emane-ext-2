#ifndef EMANEMODELSTDMAPORMANAGER_HEADER_
#define EMANEMODELSTDMAPORMANAGER_HEADER_

#include <string>
#include <map>
#include <tuple>

namespace EMANE
{
  namespace Models
  {
    namespace HeavyBall
    {
      /**
       * @class PORManager
       *
       * @brief POR Manager responsible for loading PCR curves from
       * file and determining POR.
       *
       * PCR curves are defined per data rate, with the first curve
       * defined also serving as the default curve used when a POR is
       * requested for an undefined data rate.
       */
      class PORManager
      {
      public:
        PORManager();

        void load(const std::string & sPCRFileName);

        float getPOR(std::uint64_t u64DataRate,
                     float fSINR,
                     size_t packetLengthBytes);

        using CurveDump = std::map<float,float>;
        using CurveDumps =  std::map<std::uint64_t,CurveDump>;
        CurveDumps dump();

      private:
        using Curve =  std::map<std::int32_t,float>; // sinr, por
        using DataRateTable = std::map<std::uint64_t, // data rate
                                       std::tuple<std::int32_t, // min SINR
                                                  std::int32_t, // max SINR
                                                  Curve>>;
        DataRateTable dataRateTable_;
        std::uint64_t u64DefaultCurveDataRate_;
        size_t modifierLengthBytes_;
      };
    }
  }
}

#endif
