#include <atomic>
#include <vector>

template<typename T>
class SPSCQueue {
public:
    explicit SPSCQueue(size_t capacity)
        : buffer_(capacity + 1),
          capacity_(capacity + 1),
          head_(0),
          tail_(0) {}

    bool push(const T& value) {
        const size_t tail = tail_.load(std::memory_order_relaxed);
        const size_t next = increment(tail);

        // Queue full?
        if (next == head_.load(std::memory_order_acquire))
            return false;

        buffer_[tail] = value;

        tail_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& value) {
        const size_t head = head_.load(std::memory_order_relaxed);

        // Queue empty?
        if (head == tail_.load(std::memory_order_acquire))
            return false;

        value = std::move(buffer_[head]);

        head_.store(increment(head), std::memory_order_release);
        return true;
    }

private:
    size_t increment(size_t idx) const {
        return (idx + 1) % capacity_;
    }

    std::vector<T> buffer_;
    const size_t capacity_;

    alignas(64) std::atomic<size_t> head_;
    alignas(64) std::atomic<size_t> tail_;
};