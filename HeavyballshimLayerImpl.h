#include "emane/nemlayer.h"
#include "emane/radioserviceuser.h"
//TODO:ADD ONE MORE FILE TO GET THE BYTES PER SECONDS SPEED.
#include "emane/commonmacheader.h"

namespace Emane
{
  class HeavyBallShimLayerImplementor : public NEMLayer,
                                        public RadioServiceUser
                                      
                                        
  public:
    virtual ~HeavyBallShimLayerImplementor() {}
    
  void processUpstreamPacket(const CommonMACHeader & header,
                             UpstreamPacket & pkt,
                             const ControlMessages & msgs = UpstreamTransport::empty) = 0;
    void sendDownstreamPacket(const CommonMACheader & header,
                               DownstreamPacket & pkt,
                               const ControlMessages & msgs = UpstramTransport::empty);
  
  protected:
    HeavyBallShimLayerImplementor(NEMId id,
                                  PlatformServiceProvider * pPlatformServiceProvider,
                                  RadioServiceProvider * pRadioServiceProvider);
                                  //TODO: SOMETHING LIKE BYTESPERSECOND FOR CHANNELSTATE
                                  
      
      
  NEMLayer{id,pPlatformServiceProvider}
  RadioServiceUser{pRadioServiceProvider}
  //TODO: New variable for channel state.
  
  
  private:
  void processUpstreamPacket(UpstramPacket & pkt, const ControlMessages & msgs);
  void sendDownstreamPacket(DownstreamPacket & pkt,
                            const ControlMessages & msgs = DownstreamTransport::empty);
  
  
};

typedef HeavyballShimLayerImplementor * ( *createShimFucnc)(NEMId id,
                                                           PlatformServiceProvider *p,
                                                           RadioServiceProvider * r);
typedef void(*derstroyShimFunc)(ShimLayerImplementor*);
}
#include "HeavyballshimLayerImpl.inl"
