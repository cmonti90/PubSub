#ifndef A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80
#define A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80

#include "Component.h"
#include <vector>
#include <thread>
#include <pthread.h>

namespace PubSub
{
    typedef std::vector<Component *> ComponentList;
    class Thread
    {
    public:
        enum ThreadState
        {
            INITIALIZE,
            UPDATE,
            FINALIZE
        };

        Thread();
        virtual ~Thread();
        Thread(Thread &&obj);

        Thread &operator=(Thread &&obj);

        virtual void run(const ThreadState &state);
        void stop();

        void addComp(Component *comp);
        void join();

        virtual unsigned int getProcessCount() const { return m_procs.size(); }
        void resetProcessCount() { procIdx = 0u; }

        virtual void passSubscriptionLists();

    protected:
        unsigned int procIdx{0u};
        ComponentList m_procs;
        std::thread thread;

    private:
        Thread(const Thread &) = delete;
        Thread &operator=(const Thread &) = delete;
    };

} // namespace PubSub

#endif /* A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80 */
