#ifndef DC26027A_04B1_4A1D_B196_A076A42F9DFD
#define DC26027A_04B1_4A1D_B196_A076A42F9DFD

#include "Component.h"
#include "Thread.h"
#include "QueueMngr.h"
#include "Time.h"
#include <thread>
#include <vector>
#include <memory>

namespace PubSub
{
    typedef std::vector<Thread> ThreadList;
    class Module
    {
      public:
        Module();
        Module( const std::shared_ptr< QueueMngr >& queueMngr );
        virtual ~Module() = default;

        void addThread( Thread& thread );
        void addCompToThread( Component* comp );

        void initialize();
        void iterate();
        void stop( bool over_ride = false );
        void finalize();

        virtual void launch() = 0;

      protected:

        unsigned int m_threadCount{0u};
        unsigned int maxProcCount{0u};

        ThreadList m_threads;

        std::shared_ptr<QueueMngr> m_queueMngr;

        std::shared_ptr<Time> m_time;

      private:
        void run( const ThreadBase::ThreadState& threadState );
        void runSW( const ThreadBase::ThreadState& threadState );

        Module( const Module& ) = delete;
        Module& operator=( const Module& ) = delete;
    };
} // namespace PubSub

#endif /* DC26027A_04B1_4A1D_B196_A076A42F9DFD */
