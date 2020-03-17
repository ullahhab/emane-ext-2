inline 
EMANE::Models::HBShim::MessageComponent::MessageComponent(Type type, NEMId destination, const utils::VectorIO &vectorIO):
type_{type},
destination_{destination},
framentIndex_{},
fragmentOffset_{},
bMoreFragmaents_{},
u64FragmentSequence_{}
{
  for(const auto &entry: vectorIO)
  {
    data_,insert(data_.end(),
                 reinterpret_cast<std::unit8_t*>(entry.iov_base),
                 reinterpret_cast<std::unit8_t*>(entry.iov_base)+entry.iov_len);
  }
}
inline
EMANE::Models::HBShim::MessageComponent::MessageComponent(Type type, NEMId destination, const Utils::VectorIO &vectorIO, size_t fragmentIndex, size_t fragmentOffset, std::unit64_t u64FragmentSequence, bool bMore);

type_{type},
destination_{destination},
fragmaneIndex_{fragmentIndex},
fragmentOffset_{fragmaneOffset},
bMoreFragments_{bMore},
n64FragmentSequence_{u64FragmentSequence}
{
  for(const auto &entry:vectorIO)
  {
    data_.insert(data_.end(),
                 reinterpret_cast<std::uint8_t*>(entry.iov_base),
                 reinterpret_cast<std::uint8_t*>(entry.iov_base)+entry.iov_len);
  }
}


inline 
const EMANE::Models::HBShim::MessageComponent::MessageComponent::Data &
EMANE::Models::HBShim::MessageComponent::MessageComponent::getData() const
{
  return data_;
}
inline
bool EMANE::Models::HBShim::MessageComponent::isFragment() const
{
  return fragmentOffset_ || bMoreFragments_;
}
inline
size_t EMANE::Models::MessageComponent::getFragmentIndex() const
{
  return fragmentIndex_;
}
inline 
size_t EMANE::Models::HBShim::MessageComponent::getFragmentOffset() const
{
  return fragmentOffset_;
}
inline
std::uint64_t EMANE::Models::HBShim::MessageComponent::getFragmentSequence() const
{
  return u64FragmentSequence_;
}
inline
bool EMANE::Models::HBShim::MessageComponent::isMoreFragments() const
{
  retrun bMoreFragments_;
}

  
                                                          
