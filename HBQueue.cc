#include "HBQueue.h"
#include "emane/models/tdma/queue.cc"

EMANE::Models::HeavyBall::Queue::getDestQueueLength()
{
  std::map<std::uint64_t, size_t> destQueueLength{};
  for (auto it=destQueue_.begin(); it!=destQueue_.end(); ++it)
  {
    destQueueLength.insert(std::make_pair(it->first, it->second.size()));
  }
  
  return destQueueLength;
}
