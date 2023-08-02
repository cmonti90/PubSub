#ifndef D2C30EC4_7B15_4E5E_9288_151C76998994
#define D2C30EC4_7B15_4E5E_9288_151C76998994

#include <queue>
#include <mutex>
#include <condition_variable>

namespace PubSub
{
    template <class T>
    class QueueMngr
    {
    public:
        QueueMngr() = default;
        ~QueueMngr() = default;

        void push(const T *value)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_queue.push(value);
            lock.unlock();
            // m_condition.notify_one();
        }

        T *popFront()
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            while (m_queue.empty())
            {
                m_condition.wait(lock);
            }

            T *value = m_queue.front();
            m_queue.pop();
            return value;
        }

    private:
        std::queue<T *> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_condition;
    };
} // namespace PubSub

#endif /* D2C30EC4_7B15_4E5E_9288_151C76998994 */
