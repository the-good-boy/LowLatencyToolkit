# LowLatencyToolkit

A personal systems / low-latency C++ practice repository built from queue experiments already created in my CPPLowLatency workspace.

## What this repo contains
This repository showcases practical queue and concurrency patterns that are useful for low-latency systems work:

- `LinkedListSPSCQueue.cpp` — a blocking single-producer / single-consumer queue
- `SPSCQueue.cpp` — a lock-free SPSC ring-buffer style queue
- `MPSCQueueArray.cpp` — a multi-producer / single-consumer array queue
- `MemoryPooledQueue.cpp` — a queue that reuses memory nodes for lower allocation pressure


These examples demonstrate:
- familiarity with lock-free and blocking queue design
- understanding of memory reuse and allocation behavior
- experience with concurrency primitives such as atomics, mutexes, and condition variables
- practical systems programming patterns used in performance-critical code

## Notes
This project is intentionally focused on practice and learning, using the queue implementations I already built in my CPPLowLatency folder as proof of hands-on systems work.
