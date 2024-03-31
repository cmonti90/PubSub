#ifndef A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80
#define A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80

// #include "ThreadBase.h"
#include "Component.h"
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>

namespace PubSub
{

    typedef std::string ThreadName;
    typedef std::vector< Component* > ComponentList;


    class Thread
    {
      public:

        Thread() = delete;
        Thread( const ThreadName& name );
        virtual ~Thread();

        Thread( Thread &obj );

      protected:

        friend class Module;

        enum ThreadState
        {
            INITIALIZE,
            UPDATE,
            FINALIZE
        };

        virtual void Run( const ThreadState& state );

        void AddComp( Component* comp );

        ThreadName GetThreadName() const;

        void RunInitialize();
        void RunUpdate();
        void RunFinalize();

        mutable std::mutex mtx;
        mutable std::condition_variable cv;

        ComponentList m_procs;

      private:

        const ThreadName m_threadName;

        Thread( const Thread& )             = delete;
        Thread& operator=( const Thread& )  = delete;
    };

} // namespace PubSub

#endif /* A9994F3D_1E13_4D24_9ADD_1CA6D4A84F80 */
