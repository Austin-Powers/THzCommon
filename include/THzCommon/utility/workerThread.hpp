#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

/// @brief Instances and code common for running a worker thread.
struct WorkerThread
{
    /// @brief Shortcut to the mutex used by this class.
    using Mutex = std::mutex;

    /// @brief Shortcut to the unique_lock used by this class.
    using UniqueLock = std::unique_lock<Mutex>;

    /// @brief The thread instance.
    std::thread thread{};

    /// @brief The flag to trigger shutdown of the thread.
    std::atomic_bool shutdownFlag{false};

    /// @brief The mutex of the thread.
    std::mutex mutex{};

    /// @brief The condition variable to wake up the thread.
    std::condition_variable wakeUp{};

    /// @brief Shuts down this thread.
    void shutdown() noexcept
    {
        shutdownFlag = true;
        wakeUp.notify_one();
        if (thread.joinable())
        {
            thread.join();
        }
    }
};
