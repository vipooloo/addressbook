#ifndef UNIQUEIDGENERATOR_H
#define UNIQUEIDGENERATOR_H

#include <atomic>
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace BW {

class UniqueIdGenerator
{
  public:
    UniqueIdGenerator(uint64_t min_val, uint64_t max_val)
      : min_val_(min_val)
      , max_val_(max_val)
      , current_(min_val)
    {
        if (min_val >= max_val)
        {
            throw std::invalid_argument("Minimum value must be less than maximum value");
        }
    }

    explicit UniqueIdGenerator(uint64_t max_val = std::numeric_limits<uint64_t>::max())
      : UniqueIdGenerator(0, max_val)
    {
        if (max_val == 0)
        {
            throw std::invalid_argument("Maximum value cannot be zero");
        }
    }

    // 禁用拷贝和赋值
    UniqueIdGenerator(const UniqueIdGenerator&) = delete;
    UniqueIdGenerator& operator=(const UniqueIdGenerator&) = delete;

    uint64_t next() noexcept
    {
        uint64_t current = current_.load(std::memory_order_relaxed);
        uint64_t next;

        do
        {
            current = current_.load(std::memory_order_acquire);
            next = (current >= max_val_) ? min_val_ : current + 1;
        } while (!current_.compare_exchange_weak(
            current,
            next,
            std::memory_order_release,
            std::memory_order_relaxed));

        return next;
    }

    // 新增实用功能
    uint64_t current_value() const noexcept
    {
        return current_.load(std::memory_order_relaxed);
    }

    void reset() noexcept
    {
        current_.store(min_val_, std::memory_order_release);
    }

  private:
    const uint64_t min_val_;
    const uint64_t max_val_;
    std::atomic<uint64_t> current_;
};

}  // namespace BW

#endif  // UNIQUEIDGENERATOR_H