#ifndef A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80
#define A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80

#include "ThreadBase.h"
#include "Component.h"
#include <vector>

namespace PubSub
{
    typedef std::vector<Component *> ComponentList;
    class Thread : public ThreadBase
    {
    public:
    
        Thread();
        virtual ~Thread();
        Thread(Thread &&obj);

        Thread &operator=(Thread &&obj);

        virtual void run(const ThreadState &state);

        void addComp(Component *comp);

        virtual unsigned int getProcessCount() const { return m_procs.size(); }
        void resetProcessCount() { procIdx = 0u; }

        virtual void passSubscriptionLists() override;

    protected:
        ComponentList m_procs;

    private:
        Thread(const Thread &) = delete;
        Thread &operator=(const Thread &) = delete;
    };

} // namespace PubSub

#endif /* A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80 */
