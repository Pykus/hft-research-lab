# Lesson 03 — Processes and Threads in C++20

A standalone, beginner-friendly introduction to concurrency for low-latency software.

## Learning goals

Understand the difference between a program, a process, and a thread; recognize what memory threads share; start and join threads; pass values and pointers; and identify a data race.

## 1. Program, process, and thread

A **program** is executable code stored on disk. A **process** is a running instance of that program with its own virtual address space. A **thread** is an independent path of execution inside a process.

One program may have multiple processes. One process may have multiple threads. For example, a browser commonly runs several processes, each of which may contain several threads.

Different processes normally have distinct virtual address spaces. The same numerical pointer address in two processes does not necessarily refer to the same physical memory. Ordinary pointers cannot directly access another process's memory; inter-process communication, shared memory, or appropriate operating-system APIs are required.

Threads of the **same process** share global variables, dynamically allocated objects, and the process's virtual address space. Each thread has its own call stack and execution state. Importantly, a stack is not a security boundary: another thread can access a still-living stack object if it has a valid pointer to it.

Threads can run simultaneously on different CPU cores or take turns on one core.

## 2. Compile and run

All examples are **complete, independent programs**. Save each example separately as `main.cpp`. On Linux or WSL2 with GCC:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -O0 -g -pthread main.cpp -o demo
./demo
```

The `-pthread` option enables the appropriate threading support for this toolchain.

## 3. Your first thread

```cpp
#include <iostream>
#include <thread>

void worker() {
    std::cout << "Worker finished\n";
}

int main() {
    std::cout << "Main starts\n";

    std::thread t(worker); // Starts worker() on another thread immediately.
    t.join();              // Waits until that thread finishes.

    std::cout << "Main ends\n";
    return 0;
}
```

Expected output:

```text
Main starts
Worker finished
Main ends
```

`std::thread t(worker)` **starts** the thread; `join()` **waits** for it. If a `std::thread` object is destroyed while still joinable, the program calls `std::terminate()`. Every thread therefore needs deliberate lifetime management.

## 4. Execution order is not always predictable

```cpp
#include <iostream>
#include <thread>

void worker() {
    std::cout << "B\n";
}

int main() {
    std::cout << "A\n";

    std::thread t(worker);
    std::cout << "C\n";

    t.join();
    std::cout << "D\n";
    return 0;
}
```

The worker can print B before or after the main thread prints C. However, D is printed only after the worker finishes because `join()` waits for it. Concurrent output can interleave at a finer granularity; use synchronization if complete log records must remain together.

This is the essential difference from two ordinary, sequential function calls: the caller and the new thread may be executing **at the same time**.

## 5. Passing by value

```cpp
#include <iostream>
#include <thread>

void worker(int value) {
    value = 50; // Changes only the worker's parameter.
}

int main() {
    int x = 10;

    std::thread t(worker, x);
    t.join();

    std::cout << x << '\n'; // 10
    return 0;
}
```

Passing an `int` by value creates an independent value for the worker. This is the same C++ rule as for ordinary function calls.

## 6. Passing a pointer to a shared object

```cpp
#include <iostream>
#include <thread>

void worker(int* ptr) {
    *ptr = 50;
}

int main() {
    int x = 10;

    std::thread t(worker, &x);
    t.join();

    std::cout << x << '\n'; // 50
    return 0;
}
```

`&x` takes the address of the original object. The worker modifies that object through `*ptr`. Even though `x` is local to `main`, another thread can access it through a valid pointer. The object stays alive until after `join()`, and the main thread does not access it concurrently with the worker, so this example has no data race.

Passing `x` instead of `&x` would not compile here: `worker` requires `int*`, not `int`.

## 7. Global variables are shared between threads

```cpp
#include <iostream>
#include <thread>

int counter = 0;

void worker() {
    ++counter;
}

int main() {
    std::thread t(worker);
    t.join();

    std::cout << counter << '\n'; // 1
    return 0;
}
```

The global `counter` is one object in the process. Both threads can reach it. This example is safe because the main thread waits before reading it.

## 8. A data race: two threads modify the same object

**Intentionally incorrect code — for study, not production:**

```cpp
#include <iostream>
#include <thread>

int counter = 0;

void worker() {
    for (int i = 0; i < 100'000; ++i) {
        ++counter; // Unsynchronized shared modification: DATA RACE.
    }
}

int main() {
    std::thread t1(worker);
    std::thread t2(worker);

    t1.join();
    t2.join();

    std::cout << counter << '\n';
    return 0;
}
```

Although the loops suggest 200000 increments, `++counter` on an ordinary `int` is not an atomic operation. Both threads may read and write the same object without synchronization. In C++, this is a **data race**, which causes **undefined behavior**. Do not assume the only possible problem is a smaller final count.

Where supported, investigate with ThreadSanitizer:

```bash
g++ -std=c++20 -O1 -g -pthread -fsanitize=thread main.cpp -o race_demo
./race_demo
```

## 9. First solution: mutex (preview)

```cpp
#include <iostream>
#include <mutex>
#include <thread>

int counter = 0;
std::mutex counter_mutex;

void worker() {
    for (int i = 0; i < 100'000; ++i) {
        std::lock_guard<std::mutex> lock(counter_mutex);
        ++counter;
    }
}

int main() {
    std::thread t1(worker);
    std::thread t2(worker);

    t1.join();
    t2.join();

    std::cout << counter << '\n'; // 200000
    return 0;
}
```

The global mutex coordinates access to the global counter. A `std::lock_guard` locks the existing mutex when constructed and unlocks it automatically at the end of its scope. Only one worker at a time can execute the protected increment.

## 10. Second solution: atomic counter (preview)

```cpp
#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> counter{0};

void worker() {
    for (int i = 0; i < 100'000; ++i) {
        counter += 1; // Atomic increment: safe with multiple threads.
    }
}

int main() {
    std::thread t1(worker);
    std::thread t2(worker);

    t1.join();
    t2.join();

    std::cout << counter.load() << '\n'; // 200000
    return 0;
}
```

`std::atomic<int>` makes `counter += 1` safe when multiple threads increment the same counter. Each increment happens as one atomic operation, so no increments are lost. The `join()` calls wait for both threads to finish before printing the result.

## 11. Exercises

1. Explain the difference between two processes and two threads of one process.
2. In Section 4, why must D come after the worker has finished, but C need not?
3. Run Sections 5 and 6. Explain why their results differ.
4. Explain why a local variable in `main` can be changed by another thread through a pointer.
5. Identify the data race in Section 8 and explain why the output cannot be trusted.
6. Compile Sections 9 and 10 independently and verify their final counts.
7. Explain why `join()` is not the same as a mutex.

## 12. Relevance to HFT

Low-latency systems often separate market-data reception, order-book processing, strategy logic, and order transmission. Threads can reduce blocking and distribute work, but shared mutable state introduces synchronization costs and correctness risks. Learn thread ownership and data races before attempting lock-free queues or tuning cache behavior.

**Next:** thread lifetime and ownership, mutexes, atomics, memory ordering, and single-producer/single-consumer queues.
