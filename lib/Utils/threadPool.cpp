#include "threadPool.h"

AEPKSS::Util::ThreadPool::ThreadPool() : AEPKSS::Util::ThreadPool(thread::hardware_concurrency())
{
}

AEPKSS::Util::ThreadPool::ThreadPool(size_t concurrency)
{
    this->concurrency = concurrency;
}

void AEPKSS::Util::ThreadPool::start()
{
    for (auto i = 0; i < this->concurrency; i++)
    {
        this->threads.emplace_back(thread(&ThreadPool::loop, this));
    }
}

void AEPKSS::Util::ThreadPool::stop()
{
    {
        unique_lock<mutex> lock(this->queue_mutex);
        this->should_terminate = true;
    }

    // notify all to terminate
    this->mutex_condition.notify_all();

    for (thread &t : this->threads)
    {
        t.join();
    }
}

void AEPKSS::Util::ThreadPool::loop()
{
    while (true)
    {
        function<void()> job;

        {
            // create lock
            unique_lock<mutex> lock(this->queue_mutex);

            // wait while jobQueue is empty and process should not terminate
            this->mutex_condition.wait(lock, [this]
                                       { return !this->jobs.empty() || this->should_terminate; });

            // exit if shoud_terminate
            if (this->should_terminate)
                return;

            // accquire next job
            job = this->jobs.front();
            this->jobs.pop();

            // unlock and execute job
        }

        job();
    }
}

void AEPKSS::Util::ThreadPool::submit(const function<void()> &job)
{
    {
        unique_lock<mutex> lock(this->queue_mutex);
        this->jobs.push(job);
    }
    this->mutex_condition.notify_one();
}

bool AEPKSS::Util::ThreadPool::isBusy()
{
    bool busy;
    {
        unique_lock<mutex> lock(this->queue_mutex);
        busy = !this->jobs.empty();
    }
    return busy;
}