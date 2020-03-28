inline
EMANE::Events::HeavyBall::HeavyBall(NEMId nemId,
                                    std::uint64_t weight
                                    float beat):

nemId_{nemId},
weight_{weight},
beta_{beta}

inline
EMANE::NEMId EMANE::Events::HeavyBall::getNEMId() const
{
  return nemId_;
}

inline
std::unit64_t EMANE::Events::HeavyBall::getWeight() const
{
  return weight_;
}

inline
float EMANE::Events::HeavyBall::getbeta() const
{
  return beta_;
}
