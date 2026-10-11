# Lesson 01 — Your First C++ Order and Debugging Session

**Level:** Beginner · **Duration:** ~20–30 minutes · **Environment:** Ubuntu on WSL2, VS Code, CMake, GCC, GDB

## Learning goals

By the end, you can:
- Locate a C++ declaration, implementation, and test in a real repository.
- Explain the fields of an order and why prices are represented as integer ticks.
- Understand `const`, `enum class`, and fixed-width integer types at an introductory level.
- Read boolean validation with `&&` and `!`.
- Use breakpoints and Step Over / Step Into / Step Out.
- Predict what happens when an order is invalid.

## 1. Meet the actual code

Open these files in the repository:

| File | Purpose |
| --- | --- |
| [`include/hft/order.hpp`](../../include/hft/order.hpp) | Declares the `Order` type and function interfaces |
| [`src/order.cpp`](../../src/order.cpp) | Implements order validation and formatting |
| [`src/main.cpp`](../../src/main.cpp) | Creates an order, validates it, prints it |
| [`tests/order_test.cpp`](../../tests/order_test.cpp) | Checks expected behavior |

The order type is:

```cpp
enum class Side { buy, sell };

struct Order {
    std::uint64_t id;
    Side side;
    std::int64_t price_ticks;
    std::uint64_t quantity;
};
```

- `struct` groups related data.
- `std::uint64_t` is a 64-bit unsigned integer (cannot represent negative numbers).
- `std::int64_t` is a 64-bit signed integer.
- `enum class Side` restricts the side to named alternatives (`buy` and `sell`).
- `price_ticks` stores a count of minimum price increments, **not a currency amount**. To interpret it as money, we need the instrument's tick size, which this initial model does not yet define.

## 2. Create and validate an order

In [`src/main.cpp`](../../src/main.cpp), the example creates:

```cpp
const hft::Order order{1, hft::Side::buy, 4200000, 10};
if (!hft::is_valid(order)) return 1;
```

`const` means this local order cannot be modified after initialization.

The validator in [`src/order.cpp`](../../src/order.cpp) checks:

```cpp
return order.id > 0 &&
       order.price_ticks > 0 &&
       order.quantity > 0;
```

All three conditions must be true. `&&` means logical AND; `!` means logical NOT. C++ also supports `and` and `not`, but this repository uses the more common symbolic operators.

If validation fails, `main()` returns `1` (nonzero exit status). If it succeeds, the program prints the order and returns `0`.

## 3. Debug the real program

1. Configure/build the project in **Debug** mode using CMake Tools.
2. Open `src/main.cpp` and set a breakpoint on `if (!hft::is_valid(order)) return 1;`.
3. Launch **CMake: Debug** from the Command Palette. You can also use its assigned keyboard shortcut.
4. Press **F11 (Step Into)** to enter `is_valid()` in `src/order.cpp`.
5. Inspect `order.id`, `order.price_ticks`, and `order.quantity` in **Locals**.
6. Use **F10 (Step Over)** to execute the current source-level step, or **Shift+F11 (Step Out)** to finish the current function.
7. Use **F5 (Continue)** to resume to the next breakpoint or program exit.

**Important:** F10 executes the current statement without stepping into called functions. F11 tries to step into a call. Actual stopping points can depend on debug symbols and compiler optimization.

## 4. Build and test

From the repository root in Ubuntu:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j 2
ctest --test-dir build --output-on-failure
./build/hft_demo
echo $?
```

Expected: CTest passes, the demo prints a `BUY` order, and the shell exit code is `0`.

## 5. Key takeaways

- Headers declare interfaces; `.cpp` files usually contain implementations.
- Integer tick counts avoid floating-point rounding in price representation, but tick size must be defined before converting to real prices.
- Validation rejects zero IDs, nonpositive prices, and zero quantities.
- Debugging is about understanding control flow and inspecting state, not just finding crashes.
- **Next lesson:** function arguments, `const Order&`, copies, references, and performance.

Proceed to [Exercises](EXERCISES.md).
