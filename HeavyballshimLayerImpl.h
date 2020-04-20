#include "emane/nemlayer.h"
#include "emane/radioserviceuser.h"
//TODO:ADD ONE MORE FILE TO GET THE BYTES PER SECONDS SPEED.
#include ""

namespace Emane
{
  class HeavyBallShimLayerImplementor : public NEMLayer,
                                        public RadioServiceUser,
                                        public channel_state
                                        
  public:
    virtual ~HeavyBallShimLayerImplementor() {}
  
  protected:
    HeavyBallShimLayerImplementor(NEMId id,
                                  PlatformServiceProvider * pPlatformServiceProvider,
                                  RadioServiceProvider * pRadioServiceProvider,
                                  //TODO: SOMETHING LIKE BYTESPERSECOND FOR CHANNELSTATE
                                  NONE);
      
      
  NEMLayer{id,pPlatformServiceProvider}
  RadioServiceUser{pRadioServiceProvider}
  //TODO: New variable for channel state.
  {}
};

typedef HeavyballShimLayerImplementor * ( *createShimFucnc)(NEMId id,
                                                           PlatformServiceProvider *p,
                                                           RadioServiceProvider * r);
typedef void(*derstroyShimFunc)(ShimLayerImplementor*);
}
