#include "HBbasemodelmessage.pb.h"

inline 
EMANE::Models::HeavyBall::BaseModelMessage::BaseModelMessage():
//TODO: Something would go here!

//TODO: Variable would go here! such as queue length and channel data! That's what we want right?

inline
EMANE::Models::HeavyBall::BaseModelMessage::BaseModelMessage(MessageComponents && messages):
messages_{std::move(messages)}{}

inline
//TODO: Non default constructor implementation.
  
inline
EMANE::Models::HeavyBall::BaseModelMessage::BaseModelMessage(const void * p, size_t len)
{
   EmaneMessage::TDMABaseModelMessage message;
   if(!message.PareseFromArray(p,len)){
     throw SerailizationException("unable to deserialize HBBaseModelMessage");
   }
  
  //TODO: variable that we decleared here be assigned here. 
  
  for(const auto & msg : message.messages())
  {
    MessageComponent::Type type;
 
       switch(msg.type())
         {
         case EMANEMessage::HBBaseModelMessage::Message::DATA:
           type = MessageComponent::Type::DATA;
           break;
         case EMANEMessage::HBBaseModelMessage::Message::CONTROL:
           type = MessageComponent::Type::CONTROL;
           break;
         default:
           throw SerializationException("TDMABaseModelMessage unkown type");
         }
 
       if(msg.has_fragment())
         {
           const auto & fragment = msg.fragment();
 
           messages_.push_back({type,
                 static_cast<NEMId>(msg.destination()),
                 static_cast<Priority>(msg.priority()),
                   {Utils::make_iovec(const_cast<char *>(msg.data().c_str()),
                                      msg.data().size())},
                 fragment.index(),
                   fragment.offset(),
                   fragment.sequence(),
                   fragment.more()});
         }
       else
         {
           messages_.push_back({type,
                 static_cast<NEMId>(msg.destination()),
                 static_cast<Priority>(msg.priority()),
                   {Utils::make_iovec(const_cast<char *>(msg.data().c_str()),
                                      msg.data().size())}});
         }
     }
 }

inline
const EMANE::Models::HeavyBall::MessageComponents &
EMANE::Models::HeavyBall::HBBaseModelMessage::getMessages() const
{
  return messages_;
}
inline
EMANE::Serialization EMANE::Models::HeavyBall::BaseModelMessage::serialize() const
{
  Serializtion serialization{};
  EMANEMessage::HBBasemodelMessage HBbaseModelMessage{};
  
  for(const auto & message : messages_)
  {
   //TODO:Message implementation would go here. Depends on how you  
  }
