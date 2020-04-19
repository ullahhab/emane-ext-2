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
                                  NONE)
      
      
   NEM
