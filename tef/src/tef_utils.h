#pragma once

// STD
#include <string>
#include <vector>
#include <deque>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <cstdint>

#define no_default_constructor(CLASS) CLASS() = delete
#define no_copy_constructor(CLASS) CLASS(const CLASS&) = delete
#define no_move_constructor(CLASS) CLASS& operator= (const CLASS&) = delete
#define no_copy_move_constructor(CLASS) no_copy_constructor(CLASS); no_move_constructor(CLASS);
#define no_default_copy_move_constructor(CLASS) no_default_constructor(CLASS); no_copy_constructor(CLASS); no_move_constructor(CLASS);

namespace tef::utils
{

    template<typename ... Args>
    std::string str_format(const std::string& format, Args ... args)
    {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
        if (size_s <= 0)
        {
            // Error during formatting
            return format;
        }
        auto size = static_cast<uint64_t>(size_s);
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args ...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }

    void str_replace_in_place(std::string& s, const std::string& from, const std::string& to);
    std::string str_replace(const std::string& s, const std::string& from, const std::string& to);
    std::string str_from_time();

    const char* cstr_from_bool(bool v);

    template <typename T>
    void vec_clear(std::vector<T>& vec)
    {
        vec.clear();
        std::vector<T>().swap(vec);
    }

    template <typename T>
    void vec_insert_contents(std::vector<T>& a, const std::vector<T>& b)
    {
        a.reserve(a.size() + b.size());
        a.insert(a.end(), b.begin(), b.end());
    }

    template <typename T>
    void vec_remove(std::vector<T>& vec, size_t pos)
    {
        vec.erase(vec.begin() + pos);
    }

    template <typename T>
    ptrdiff_t vec_find_index(const std::vector<T>& vec, const T& value)
    {
        for (size_t i = 0; i < vec.size(); i++)
        {
            if (value == vec[i])
            {
                return i;
            }
        }
        return -1;
    }

    template <typename T>
    bool vec_contains(const std::vector<T>& vec, const T& value)
    {
        return vec_find_index(vec, value) > -1;
    }

    float elapsed_sec(std::chrono::steady_clock::time_point t_start);

    float elapsed_sec(
        std::chrono::steady_clock::time_point t_start,
        std::chrono::steady_clock::time_point t_end
    );

    // Spawns a worker thread upon construction and keeps waiting for new jobs to process (using a
    // FIFO queue). Upon deconstruction, the worker thread will automatically stop after the queue
    // is empty.
    class worker_t
    {
    public:
        const uint64_t id;

        worker_t(uint64_t id);
        no_default_copy_move_constructor(worker_t);
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

    // Pseudo-Random Number Generator
    class prng_t
    {
    public:
        prng_t();
        no_copy_move_constructor(prng_t);

        int32_t next_i32();
        int32_t next_i32(int32_t min, int32_t max);

        uint32_t next_u32();
        uint32_t next_u32(uint32_t min, uint32_t max);

        int64_t next_i64();
        int64_t next_i64(int64_t min, int64_t max);

        uint64_t next_u64();
        uint64_t next_u64(uint64_t min, uint64_t max);

        float next_float();
        float next_float(float min, float max);

        double next_double();
        double next_double(double min, double max);

    private:
        // Engine
        std::random_device device;
        std::mt19937 engine;

        // Distributors
        std::uniform_int_distribution<int32_t> dist_i32;
        std::uniform_int_distribution<uint32_t> dist_u32;
        std::uniform_int_distribution<int64_t> dist_i64;
        std::uniform_int_distribution<uint64_t> dist_u64;
        std::uniform_real_distribution<float> dist_float;
        std::uniform_real_distribution<double> dist_double;

    };

}
