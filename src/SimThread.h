#ifndef D71B08DE_3F7C_44EE_A271_1802F0E690B3
#define D71B08DE_3F7C_44EE_A271_1802F0E690B3

#include "SimComponent.h"
#include "Thread.h"
#include <vector>
#include <thread>

namespace PubSub
{
    typedef std::vector<SimComponent *> SimComponentList;
    class SimThread : public Thread
    {
    public:

        SimThread();
        virtual ~SimThread();
        SimThread(SimThread &&obj);

        SimThread &operator=(SimThread &&obj);

        void run(const ThreadState &state, unsigned int counter);

        void addComp(SimComponent *comp);

        unsigned int getProcessCount() const { return m_simProcs.size(); }

    private:
        SimComponentList m_simProcs;

        SimThread(const SimThread &) = delete;
        SimThread &operator=(const SimThread &) = delete;
    };

} // namespace PubSub

#endif /* D71B08DE_3F7C_44EE_A271_1802F0E690B3 */
