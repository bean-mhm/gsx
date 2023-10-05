#pragma once

// STD
#include <deque>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdint>

// Internal
#include "../internal_common/all.h"

namespace tef::misc
{

    // Spawns a worker thread upon construction and keeps waiting for new jobs to process (using a
    // FIFO queue). Upon deconstruction, the worker thread will automatically stop after the queue
    // is empty.
    class worker_t
    {
    public:
        const u64 id;

        worker_t(u64 id);
        no_default_copy_construct_no_assignment(worker_t);
        ~worker_t();

        // Enqueue a new job to be processed by the worker thread.
        void enqueue(const std::function<void()>& job);

        // Wait for the worker thread to process all jobs until the queue is empty.
        void wait();

    private:
        std::jthread thread;

        std::deque<std::function<void()>> jobs;
        std::mutex jobs_mutex;

        std::condition_variable cond_job_added;
        std::condition_variable cond_queue_empty;

        void loop(std::stop_token stop_token);

    };

}
