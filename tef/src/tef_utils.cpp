#include "tef_utils.h"

// STD
#include <time.h>

namespace tef::utils
{
    // https://stackoverflow.com/a/3418285/18049911
    void str_replace_in_place(std::string& s, const std::string& from, const std::string& to)
    {
        if (from.empty())
            return;

        uint64_t start_pos = 0;
        while ((start_pos = s.find(from, start_pos)) != std::string::npos)
        {
            s.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }

    std::string str_replace(const std::string& s, const std::string& from, const std::string& to)
    {
        std::string s_copy = s;
        str_replace_in_place(s_copy, from, to);
        return s_copy;
    }

    std::string str_from_time()
    {
        time_t raw_time;
        tm time_info;
        char buffer[128];

        time(&raw_time);
        localtime_s(&time_info, &raw_time);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);

        return std::string(buffer);
    }

    const char* cstr_from_bool(bool v)
    {
        return v ? "true" : "false";
    }

    float elapsed_sec(std::chrono::steady_clock::time_point t_start)
    {
        return 1e-9f * std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - t_start).count();
    }

    float elapsed_sec(
        std::chrono::steady_clock::time_point t_start,
        std::chrono::steady_clock::time_point t_end
    )
    {
        return 1e-9f * std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();
    }

    worker_t::worker_t(uint64_t id)
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

    prng_t::prng_t()
        : device(),
        engine(device()),
        dist_i32(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()),
        dist_u32(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()),
        dist_i64(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()),
        dist_u64(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()),
        dist_float(0.0f, 1.0f),
        dist_double(0.0, 1.0)
    {}

    int32_t prng_t::next_i32()
    {
        return dist_i32(engine);
    }

    int32_t prng_t::next_i32(int32_t min, int32_t max)
    {
        int32_t count = (max - min) + 1;
        return min + (((next_i32() % count) + count) % count);
    }

    uint32_t prng_t::next_u32()
    {
        return dist_u32(engine);
    }

    uint32_t prng_t::next_u32(uint32_t min, uint32_t max)
    {
        uint32_t count = (max - min) + 1;
        return min + (((next_u32() % count) + count) % count);
    }

    int64_t prng_t::next_i64()
    {
        return dist_i64(engine);
    }

    int64_t prng_t::next_i64(int64_t min, int64_t max)
    {
        int64_t count = (max - min) + 1;
        return min + (((next_i64() % count) + count) % count);
    }

    uint64_t prng_t::next_u64()
    {
        return dist_u64(engine);
    }

    uint64_t prng_t::next_u64(uint64_t min, uint64_t max)
    {
        uint64_t count = (max - min) + 1;
        return min + (((next_u64() % count) + count) % count);
    }

    float prng_t::next_float()
    {
        return dist_float(engine);
    }

    float prng_t::next_float(float min, float max)
    {
        return min + (next_float() * (max - min));
    }

    double prng_t::next_double()
    {
        return dist_double(engine);
    }

    double prng_t::next_double(double min, double max)
    {
        return min + (next_double() * (max - min));
    }

}
