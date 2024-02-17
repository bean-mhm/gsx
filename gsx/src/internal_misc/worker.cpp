#include "worker.h"

namespace gsx::misc
{

    worker_t::worker_t(u64 id)
        : id(id), thread(
            [this](std::stop_token stop_token)
            {
                loop(stop_token);
            }
        )
    {}

    worker_t::~worker_t()
    {
        // if the thread is stuck waiting for new jobs to process, lie to it
        // because the deconstructor of std::jthread will automatically call
        // request_stop() so the thread will know that it needs to stop waiting
        // for new jobs.
        cond_job_added.notify_all();
    }

    void worker_t::enqueue(const std::function<void()>& job)
    {
        {
            std::unique_lock lock(jobs_mutex);
            jobs.push_back(job);
        }
        cond_job_added.notify_all();
    }

    void worker_t::wait()
    {
        std::unique_lock lock(jobs_mutex);
        cond_queue_empty.wait(
            lock,
            [this]()
            {
                return jobs.empty();
            }
        );
    }

    void worker_t::loop(std::stop_token stop_token)
    {
        while (!stop_token.stop_requested())
        {
            // process all pending jobs until there is nothing else to do
            {
                std::unique_lock lock(jobs_mutex);
                while (!jobs.empty())
                {
                    // get the next job
                    const auto& job = jobs.front();

                    // unlock during the job
                    lock.unlock();
                    job();
                    lock.lock();

                    // pop the front element which we just processed
                    jobs.pop_front();
                }
            }

            // notify that the queue is empty
            cond_queue_empty.notify_all();

            // wait until there's something new to do, or if deconstruction is
            // happening
            std::unique_lock lock(jobs_mutex);
            cond_job_added.wait(
                lock,
                [this, stop_token]()
                {
                    return !jobs.empty() || stop_token.stop_requested();
                }
            );
        }
    }

}
