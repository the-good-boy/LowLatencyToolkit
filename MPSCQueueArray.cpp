#include <atomic>
#include <vector>

template<typename T>
class MPSCArrayQueue {
public:
    explicit MPSCArrayQueue(size_t capacity)
        : buffer_(capacity),
          ready_(capacity),
          capacity_(capacity),
          head_(0),
          tail_(0)
    {
        for (auto& r : ready_)
            r.store(false, std::memory_order_relaxed);
    }

    bool push(const T& item)
    {
        size_t pos = tail_.fetch_add(1, std::memory_order_acq_rel);

        size_t idx = pos % capacity_;

        // Slot still occupied => queue full
        if (ready_[idx].load(std::memory_order_acquire))
            return false;

        buffer_[idx] = item;

        ready_[idx].store(true, std::memory_order_release);

        return true;
    }

    bool pop(T& item)
    {
        size_t idx = head_ % capacity_;

        if (!ready_[idx].load(std::memory_order_acquire))
            return false;

        item = std::move(buffer_[idx]);

        ready_[idx].store(false, std::memory_order_release);

        ++head_;

        return true;
    }

private:
    std::vector<T> buffer_;
    std::vector<std::atomic<bool>> ready_;

    const size_t capacity_;

    alignas(64) std::atomic<size_t> tail_;
    alignas(64) size_t head_; // consumer only
};