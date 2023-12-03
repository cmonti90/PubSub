#ifndef BE7BBEAF_E799_46AD_9ED3_E63D274AB0FB
#define BE7BBEAF_E799_46AD_9ED3_E63D274AB0FB

#include "Component.h"

namespace PubSub
{
    class SimComponent : public Component
    {
    public:
        SimComponent() = delete;
        SimComponent(std::shared_ptr<QueueMngr>& queue_mngr, unsigned int rate, const Component_Label str);

        virtual ~SimComponent() = default;

        unsigned int getModelRate() const;

    private:
        friend class SimThread;

        const unsigned int model_rate;

        SimComponent(const SimComponent &) = delete;
        SimComponent &operator=(const SimComponent &) = delete;
    };
} // namespace PubSub


#endif /* BE7BBEAF_E799_46AD_9ED3_E63D274AB0FB */
