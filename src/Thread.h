#ifndef A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80
#define A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80

#include "Component.h"
#include <vector>
#include <thread>

namespace PubSub
{
    typedef std::vector<Component *> ComponentList;
    class Thread
    {
    public:

        enum class ThreadState
        {
            INITIALIZE,
            UPDATE,
            FINALIZE
        };

        Thread();
        ~Thread();
        Thread(Thread &&obj);

        Thread &operator=(Thread &&obj)
        {
            if (thread.joinable())
            {
                thread.join();
            }

            procIdx = obj.procIdx;
            m_procs = std::move(obj.m_procs);
            thread = std::move(obj.thread);

            return *this;
        }

        
        void run(const ThreadState &state);

        void addComp(Component *comp);
        void join();

        unsigned int getProcessCount() const { return m_procs.size(); }
        void resetProcessCount() { procIdx = 0u; }

    private:
        unsigned int procIdx{0u};
        ComponentList m_procs;
        std::thread thread;

        Thread(const Thread &) = delete;
        Thread &operator=(const Thread &) = delete;
    };
} // namespace PubSub
#endif /* A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80 */
