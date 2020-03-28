#ifndef EMANEMODELHBSHIMMESSAGECOMPONENT_HEADER_
#define EMANEMODELHBSHIMMESSAGECOMPONENT_HEADER_

#include "emane/models/HBShim/messagecomponent.inl"

//TODO: Subsidary files needed for defining and extending this class.



namespace EMANE
{
  namespace Models
  {
    namespace HBShim
    {
      class MessageComponent
      {
        public:
        enum class Type
        {
        //TODO: DATA structure needed to be defined here.
        DATA,
        CONTROL
          };
          //TODO: 
          //constructor without fragmantation of data
          MessageComponent(Type type, NEMId destination, const Utils::VectorIO & vectorIO);
          
          MessageComponent(Type typem NEMId destination, constUtils::VectorIO &vectorIO, size_t fragmentIndex, size_t fragmentOffset, std::uint64_t u64FragmentSequence, bool bMore);
          
          const Data & getData() const;
          NEMId getDestination() const;
          Type getType() const;
          bool isFragment() const;
          size_t getFragmentIndex() const;
          size_t getFragmentOffset() const;
          std::uint64_t getFragments() const;
          bool isMoreFragments() const;
          //TODO: Maybe getWeight function to get the weight. Will figure out as we move along.
          
          
          private:
          Type type_;
          NEMId destination_;
          //TODO: Maybe variable for weight if getWeight() function is developed here.
          size_t fragmentIndex_;
          size_t fragmaneOffset_;
          bool bMoreFragments_;
          std::unit64_t u64FragmentSequence_;
          };
          using MessageComponenets = std::list<MessageComponent>;
         }
      }
}
      
          
      
          
