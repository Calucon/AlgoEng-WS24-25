#include <cstdlib>
#include <vector>
#include <queue>
#include <future>
#include <thread>

using namespace std;

namespace AEPKSS::Util
{
    /**
     * Simple thread pool implementation
     * @see https://stackoverflow.com/a/32593825
     */
    class ThreadPool
    {
    private:
        size_t concurrency;
        bool should_terminate = false;      // Tells threads to stop looking for jobs
        mutex queue_mutex;                  // Prevents data races to the job queue
        condition_variable mutex_condition; // Allows threads to wait on new jobs or termination
        vector<thread> threads;
        queue<function<void()>> jobs;

        void loop();

    public:
        ThreadPool();
        ThreadPool(size_t concurrency);

        /**
         * Adds a ob to the pool
         */
        void submit(const function<void()> &job);

        /**
         * Busy returns true as long as there are jobs queued
         */
        bool isBusy();

        /**
         * Starts execution of the thread pool
         */
        void start();

        /**
         * Stops execution and waits for all currently running jobs to finish
         */
        void stop();
    };
}