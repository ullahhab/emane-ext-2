#include "HBbasemodelmessage.pb.h"

inline 
EMANE::Models::HeavyBall::BaseModelMessage::BaseModelMessage():
//TODO: Something would go here!


inline
EMANE::Models::HeavyBall::BaseModelMessage::BaseModelMessage(MessageComponents && messages):
messages_{std::move(messages)}{}

inline
EMANE::Models::HeavyBall::
