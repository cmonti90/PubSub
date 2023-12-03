#ifndef D71B08DE_3F7C_44EE_A271_1802F0E690B3
#define D71B08DE_3F7C_44EE_A271_1802F0E690B3

#include "ThreadBase.h"
#include "SimComponent.h"
#include <vector>

namespace PubSub
{
    typedef std::vector<SimComponent*> SimComponentList;
    class SimThread : public ThreadBase
    {
      public:

        SimThread();
        virtual ~SimThread();
        SimThread( SimThread &&obj );

        SimThread& operator=( SimThread &&obj );

        virtual void run( const ThreadState& state, unsigned int counter );

        void addComp( SimComponent* comp );

        virtual unsigned int getProcessCount() const override
        {
            return m_simProcs.size();
        }

      protected:
        SimComponentList m_simProcs;

      private:

        SimThread( const SimThread& ) = delete;
        SimThread& operator=( const SimThread& ) = delete;
    };

} // namespace PubSub

#endif /* D71B08DE_3F7C_44EE_A271_1802F0E690B3 */
