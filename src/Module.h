#ifndef DC26027A_04B1_4A1D_B196_A076A42F9DFD
#define DC26027A_04B1_4A1D_B196_A076A42F9DFD

#include "Component.h"
#include "Thread.h"
#include "QueueMngr.h"
#include "Time.h"
#include <thread>
#include <vector>
#include <memory>
#include <unordered_map>

namespace PubSub
{
    typedef std::unordered_map< ThreadName, Thread* > ThreadList;

    class Module
    {
      public:

        Module();
        Module( const std::shared_ptr< QueueMngr >& queueMngr );
        virtual ~Module() = default;

        void RegisterThread( Thread& thread );
        void AddCompToThread( Thread& thread, Component* comp );

        void initialize();
        void iterate();
        void stop( bool over_ride = false );
        void finalize();

        virtual void launch() = 0;

      protected:

        unsigned int m_numThreadsActive;

        ThreadList m_threads;

        std::shared_ptr< QueueMngr > m_queueMngr;
        
        std::shared_ptr< Time > m_time;

      private:

        static constexpr unsigned int MAX_THREADS = 4u;

        void Run( const Thread::ThreadState& threadState );
        void RunSW( const Thread::ThreadState& threadState );
        void KickoffThreads( const Thread::ThreadState& threadState );
        void RunThread( Thread* thread, const Thread::ThreadState& threadState );
        void JoinThread( Thread* thread );
        void JoinAllThreads();

        std::thread* m_thread;

        Module( const Module& ) = delete;
        Module& operator=( const Module& ) = delete;
    };
} // namespace PubSub

#endif /* DC26027A_04B1_4A1D_B196_A076A42F9DFD */
