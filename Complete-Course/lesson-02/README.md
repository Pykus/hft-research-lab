# Lesson 02 — References, Pointers, and Object Lifetime

**Level:** Beginner | **Duration:** 15–20 minutes

Examples use [`hft::Order`](../../include/hft/order.hpp) from this repository. These are teaching snippets, not changes to the application.

## 1. Value vs reference

```cpp
void by_value(hft::Order order) {
    order.quantity = 100; // Changes a copy.
}
void by_reference(hft::Order& order) {
    order.quantity = 100; // Changes the original.
}
bool is_nonempty(const hft::Order& order) {
    return order.quantity > 0; // Cannot modify through this reference.
}
```

| Parameter | Copies Order? | Can modify original? |
| --- | --- | --- |
| `Order order` | Yes | No |
| `Order& order` | No | Yes |
| `const Order& order` | No | No, not through this reference |

For small types, passing by value can be efficient; benchmark before optimizing.

## 2. Pointers and copies

```cpp
hft::Order order{1, hft::Side::buy, 4200000, 10};
hft::Order copy = order;
hft::Order* ptr = &order;

copy.quantity = 20;
ptr->quantity = 30;

std::cout << order.quantity << '\n'; // 30
std::cout << copy.quantity << '\n';  // 20
```

`&order` obtains the address; `ptr` stores it. `ptr->quantity` accesses the original object. `copy` is independent.

## 3. Null and dangling pointers

```cpp
hft::Order* ptr = nullptr;
if (ptr != nullptr) {
    std::cout << ptr->quantity << '\n';
}
```

Dereferencing `nullptr` is undefined behavior.

```cpp
hft::Order* ptr = nullptr;
{
    hft::Order order{1, hft::Side::buy, 4200000, 10};
    ptr = &order;
} // order is destroyed here.

std::cout << ptr->quantity << '\n'; // Undefined behavior!
```

The pointer still exists, but the object it points to does not. Do not execute the invalid access as a test.

## 4. Unique ownership

```cpp
#include <memory>
#include <utility>

auto first = std::make_unique<hft::Order>(
    hft::Order{1, hft::Side::buy, 4200000, 10}
);
auto second = std::move(first);

std::cout << second->quantity << '\n'; // 10
// first is now empty; do not dereference it.
```

`std::unique_ptr` automatically destroys its owned object. It cannot be copied; moving transfers ownership without copying the `Order`. In HFT, measure allocations and avoid unnecessary work on latency-critical paths.

## Exercises

1. Predict the original quantity after calling `by_value(order)` and `by_reference(order)`.
2. Explain why the pointer-and-copy example prints `30` and `20`.
3. Identify which object ends its lifetime in the dangling-pointer example.
4. After `std::move(first)`, which pointer owns the object? What is `second->quantity`?
5. In a temporary test file, create an `Order` with `std::make_unique`, move ownership, and read the quantity through the new owner.

**Checkpoint:** Explain copies, references, non-owning pointers, dangling pointers, and unique ownership.

**Next lesson:** RAII, move semantics, and allocation costs.
