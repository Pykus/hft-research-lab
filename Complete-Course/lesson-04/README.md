# Lesson 04 — Memory Layout, Cache Lines, and Data-Oriented Design

This lesson connects C++ object layout with CPU cache behavior. These ideas matter in low-latency systems because a program can be logically correct and still waste time moving unnecessary data through the cache hierarchy.

## Learning goals

By the end of this lesson, you should be able to:

- explain what a cache line is,
- recognize false sharing,
- understand alignment and padding,
- use `sizeof` and `alignof`,
- distinguish Array of Structures (AoS) from Structure of Arrays (SoA),
- explain why data locality matters in low-latency C++.

## 1. Cache lines

CPUs usually move memory through caches in fixed-size blocks called **cache lines**.

A common cache-line size on modern x86-64 processors is 64 bytes, but C++ does not guarantee that every machine uses 64-byte cache lines.

If an object is 32 bytes and a cache line is 64 bytes, two such objects may fit in one cache line.

## 2. False sharing

Two threads may modify two different atomic variables and still interfere with each other if both variables occupy the same cache line.

### Example

```cpp
#include <atomic>
#include <cstdint>
#include <iostream>
#include <thread>

struct Counters {
    std::atomic<std::uint64_t> first{0};
    std::atomic<std::uint64_t> second{0};
};

Counters counters;

void worker1() {
    for (std::uint64_t i = 0; i < 100'000'000; ++i) {
        counters.first += 1;
    }
}

void worker2() {
    for (std::uint64_t i = 0; i < 100'000'000; ++i) {
        counters.second += 1;
    }
}

int main() {
    std::thread t1(worker1);
    std::thread t2(worker2);

    t1.join();
    t2.join();

    std::cout << "first:  " << counters.first << '\n';
    std::cout << "second: " << counters.second << '\n';

    return 0;
}
```

There is no data race between `first` and `second`. Each thread modifies a different atomic object.

However, if both atomics share one cache line, the cache-coherence mechanism may repeatedly move ownership of that line between CPU cores.

This performance problem is called **false sharing**.

A simple teaching example for separating them is:

```cpp
struct Counters {
    alignas(64) std::atomic<std::uint64_t> first{0};
    alignas(64) std::atomic<std::uint64_t> second{0};
};
```

Do not treat 64 as a universal C++ constant. It is a common hardware value and useful for understanding the idea.

## 3. Alignment and padding

Consider:

```cpp
struct Order {
    char side;
    std::uint64_t id;
    char type;
};
```

The fields themselves require only:

```text
1 + 8 + 1 = 10 bytes
```

but `sizeof(Order)` may be larger because the compiler inserts **padding** to satisfy alignment requirements.

### Complete example

```cpp
#include <cstdint>
#include <iostream>

struct Order {
    char side;
    std::uint64_t id;
    char type;
};

int main() {
    std::cout << "sizeof(Order): " << sizeof(Order) << '\n';
    std::cout << "alignof(Order): " << alignof(Order) << '\n';

    return 0;
}
```

On a typical x86-64 system, a possible layout is:

```text
offset 0      side
offset 1-7    padding
offset 8-15   id
offset 16     type
offset 17-23  padding
```

That gives a total size of 24 bytes.

Exact sizes and alignments are implementation-dependent, so measure them on the platform you actually use.

## 4. Field order can change object size

Reordering the same fields may reduce padding:

```cpp
#include <cstdint>
#include <iostream>

struct Order {
    std::uint64_t id;
    char side;
    char type;
};

int main() {
    std::cout << "sizeof(Order): " << sizeof(Order) << '\n';
    std::cout << "alignof(Order): " << alignof(Order) << '\n';

    return 0;
}
```

A typical layout is now:

```text
offset 0-7    id
offset 8      side
offset 9      type
offset 10-15  padding
```

The structure may therefore occupy 16 bytes instead of 24.

This matters when millions of objects are stored in memory because smaller objects can allow more useful data to fit into CPU caches.

## 5. Array of Structures — AoS

A natural representation of orders is:

```cpp
#include <cstdint>
#include <iostream>
#include <vector>

struct Order {
    std::uint64_t id;
    double price;
    std::uint32_t quantity;
    char side;
};

int main() {
    std::vector<Order> orders{
        {1, 100.5, 10, 'B'},
        {2, 101.0, 20, 'S'},
        {3, 99.8, 15, 'B'}
    };

    for (const Order& order : orders) {
        std::cout
            << order.id << ' '
            << order.price << ' '
            << order.quantity << ' '
            << order.side << '\n';
    }

    return 0;
}
```

Conceptually, memory contains:

```text
[id price quantity side]
[id price quantity side]
[id price quantity side]
```

This layout is called **Array of Structures (AoS)**.

AoS is convenient when processing most or all fields of one object together.

## 6. Structure of Arrays — SoA

If an algorithm repeatedly processes only one field, a different layout may provide better data locality.

```cpp
#include <cstdint>
#include <iostream>
#include <vector>

struct Orders {
    std::vector<std::uint64_t> ids;
    std::vector<double> prices;
    std::vector<std::uint32_t> quantities;
    std::vector<char> sides;
};

int main() {
    Orders orders;

    orders.ids = {1, 2, 3};
    orders.prices = {100.5, 101.0, 99.8};
    orders.quantities = {10, 20, 15};
    orders.sides = {'B', 'S', 'B'};

    double total_price = 0.0;

    for (double price : orders.prices) {
        total_price += price;
    }

    std::cout << "Total price: " << total_price << '\n';

    return 0;
}
```

Conceptually:

```text
ids:        [id][id][id][id]
prices:     [price][price][price][price]
quantities: [qty][qty][qty][qty]
sides:      [side][side][side][side]
```

This is **Structure of Arrays (SoA)**.

If an algorithm needs only prices, the CPU can load densely packed prices instead of also bringing IDs, quantities, and sides into cache.

## 7. Data locality

`std::vector` stores its elements contiguously.

For example:

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> values(1'000'000, 1);

    long long sum = 0;

    for (int value : values) {
        sum += value;
    }

    std::cout << sum << '\n';

    return 0;
}
```

The integers are stored next to one another in memory.

If an `int` occupies 4 bytes and a cache line is 64 bytes, one cache line can hold approximately 16 consecutive integers.

Sequential access therefore makes good use of each cache line.

## 8. AoS or SoA?

There is no universal winner.

Use the access pattern to guide the layout.

### AoS often makes sense when

You usually process a complete object:

```cpp
process(order.id, order.price, order.quantity, order.side);
```

### SoA often makes sense when

You process one or a few fields across many objects:

```cpp
for (double price : orders.prices) {
    // Process only prices.
}
```

For example, calculating an average price across millions of trades is a good candidate for SoA because the algorithm does not need IDs or quantities.

## 9. HFT perspective

Low-latency optimization is not only about writing fewer instructions.

Memory layout affects:

- how much useful data fits in cache,
- how many cache lines must be loaded,
- whether independent threads accidentally share cache lines,
- how predictable memory access becomes.

A useful rule is:

> Store data according to how the hot path actually accesses it.

Do not choose AoS or SoA only because one looks cleaner. Measure the real workload.

## Exercises

1. A cache line is 64 bytes and `Order` is 32 bytes. How many complete `Order` objects fit in one cache line?
2. Explain why two different atomic counters can still suffer from false sharing.
3. Why can `char, uint64_t, char` require more memory than the sum of the field sizes?
4. Reorder those three fields to reduce typical padding.
5. You have one million trades and calculate only the average price. Which layout is likely to be more cache-friendly: AoS or SoA?
6. Explain why contiguous storage in `std::vector` is usually helpful for sequential processing.
