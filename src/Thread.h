#ifndef A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80
#define A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80

#include "Component.h"
#include <vector>
#include <thread>

namespace PubSub
{
    class Thread
    {
    public:
        Thread() = default;
        ~Thread() = default;

        void start();
        void stop();
        void addComp(Component *comp);
        void join();

        unsigned int getProcessCount() const { return m_procs.size(); }
        void resetProcessCount() { procIdx = 0u; }

    private:
        unsigned int procIdx{0u};
        std::vector<Component*> m_procs;
        std::thread thread;
    }
} // namespace PubSub
#endif /* A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80 */
