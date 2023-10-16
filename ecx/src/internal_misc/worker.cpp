#include "worker.h"

namespace ecx::misc
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
        // If the thread is stuck waiting for new jobs to process, lie to it. The deconstructor of
        // std::jthread will also automatically call request_stop() so the thread will know that
        // it needs to stop waiting for new jobs.
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
            // Process all pending jobs until there is nothing else to do
            {
                std::unique_lock lock(jobs_mutex);
                while (!jobs.empty())
                {
                    // Get the next job
                    const auto& job = jobs.front();

                    // Unlock during the job
                    lock.unlock();
                    job();
                    lock.lock();

                    // Pop the front element which we just processed
                    jobs.pop_front();
                }
            }

            // Notify that the queue is empty
            cond_queue_empty.notify_all();

            // Wait until there's something new to do, or if deconstruction is happening
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
