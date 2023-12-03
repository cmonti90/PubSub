
#include "Component.h"
#include "QueueMngr.h"

namespace PubSub
{
    Component::Component( std::shared_ptr<QueueMngr>& queue_mngr, const Component_Label str )
        : label( str ),
          m_queue_mngr( queue_mngr )
    {
    }

    Component_Label Component::getComponentLabel() const
    {
        return label;
    }

    bool Component::associateEvent() const
    {
        return false;
    }

} // namespace PubSub