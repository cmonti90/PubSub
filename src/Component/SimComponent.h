#ifndef BE7BBEAF_E799_46AD_9ED3_E63D274AB0FB
#define BE7BBEAF_E799_46AD_9ED3_E63D274AB0FB

#include "Component.h"

namespace PubSub
{
    class SimComponent : public Component
    {
    public:
        friend class QueueMngr;

        SimComponent() = delete;
        SimComponent(std::shared_ptr<QueueMngr>& queue_mngr, unsigned int rate, const Component_Label str)
        : Component(queue_mngr, str), model_rate(rate)
        {
        }

        virtual ~SimComponent() = default;

        unsigned int getModelRate() const
        {
            return model_rate;
        }

    private:
        const unsigned int model_rate;

        SimComponent(const SimComponent &) = delete;
        SimComponent &operator=(const SimComponent &) = delete;
    };
} // namespace PubSub


#endif /* BE7BBEAF_E799_46AD_9ED3_E63D274AB0FB */
