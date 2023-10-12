
#include "SimComponent.h"
#include "QueueMngr.h"

namespace PubSub
{
    SimComponent::SimComponent( std::shared_ptr<QueueMngr>& queue_mngr, unsigned int rate, const Component_Label str )
        : Component( queue_mngr, str ), model_rate( rate )
    {
    }

    unsigned int SimComponent::getModelRate() const
    {
        return model_rate;
    }

} // namespace PubSub