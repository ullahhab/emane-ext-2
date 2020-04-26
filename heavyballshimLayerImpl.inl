#include "emane/commonheaderexception.h"

inline
void EMANE::HeavyBallShimLayerImplementor::sendDeonstreamPacket(const CommonMACHeader & header,
                                                                DownstramPacket & pkt,
                                                                const ControlMessages & msgs)
{
  header.prepandTo(pkt);
  sendDownstramPacket(pkt,msgs);
}


inline
void EMANE::HeavyBallShimLayerImplementor::processUpstreamPacket(UpstreamPacket & pkt,
                                                                 const ControlMessages & msgs)
{
  try{
    CommonMACheader header(pkt);
    
    processUpstramPacket(header,
                         pkt,
                         msgs);
  }
    catch(commonMACHeaderException &)
    {
      throw;
    }
}
inline
void EMANE::EMANE::HeavyBallShimLayerImplementor::sendDeonstreamPacket(DownstreamPacket & pkt,
                                                                       const ControlMessages & msgs)
{
  UpstreamTransport::sendDownstreamPacket(pkt,msgs);
}
