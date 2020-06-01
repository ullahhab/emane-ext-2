inline
EMANE::Models::HeavyBall::MessageComponent::MessageComponent(Type type,
                                                        NEMId destination,
                                                        Priority priority,
                                                        const Utils::VectorIO & vectorIO):
  type_{type},
  destination_{destination},
  priority_{priority},
  fragmentIndex_{},
  fragmentOffset_{},
  bMoreFragments_{},
  u64FragmentSequence_{}
{
  for(const auto & entry : vectorIO)
    {
      data_.insert(data_.end(),
                   reinterpret_cast<std::uint8_t*>(entry.iov_base),
                   reinterpret_cast<std::uint8_t*>(entry.iov_base) + entry.iov_len);
    }
}

inline
EMANE::Models::HeavyBall::MessageComponent::MessageComponent(Type type,
                                                        NEMId destination,
                                                        Priority priority,
                                                        const Utils::VectorIO & vectorIO,
                                                        size_t fragmentIndex,
                                                        size_t fragmentOffset,
                                                        std::uint64_t u64FragmentSequence,
                                                        bool bMore):
  type_{type},
  destination_{destination},
  priority_{priority},
  fragmentIndex_{fragmentIndex},
  fragmentOffset_{fragmentOffset},
  bMoreFragments_{bMore},
  u64FragmentSequence_{u64FragmentSequence}
{
  for(const auto & entry : vectorIO)
    {
      data_.insert(data_.end(),
                   reinterpret_cast<std::uint8_t*>(entry.iov_base),
                   reinterpret_cast<std::uint8_t*>(entry.iov_base) + entry.iov_len);
    }
}

inline
const EMANE::Models::HeavyBall::MessageComponent::Data &
EMANE::Models::HeavyBall::MessageComponent::getData() const
{
  return data_;
}

inline
NEMId EMANE::Models::HeavyBall::MessageComponent::getDestination() const
{
  return destination_;
}

inline
EMANE::Models::HeavyBall::MessageComponent::Type
EMANE::Models::HeavyBall::MessageComponent::getType() const
{
  return type_;
}

inline
bool EMANE::Models::HeavyBall::MessageComponent::isFragment() const
{
  return fragmentOffset_ || bMoreFragments_;
}

inline
size_t EMANE::Models::HeavyBall::MessageComponent::getFragmentIndex() const
{
  return fragmentIndex_;
}

inline
size_t EMANE::Models::HeavyBall::MessageComponent::getFragmentOffset() const
{
  return fragmentOffset_;
}

inline
std::uint64_t EMANE::Models::HeavyBall::MessageComponent::getFragmentSequence() const
{
  return u64FragmentSequence_;
}

inline
bool EMANE::Models::HeavyBall::MessageComponent::isMoreFragments() const
{
  return bMoreFragments_;
}

inline
EMANE::Priority EMANE::Models::HeavyBall::MessageComponent::getPriority() const
{
  return priority_;
}
