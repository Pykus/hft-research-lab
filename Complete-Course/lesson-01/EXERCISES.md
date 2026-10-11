# Lesson 01 — Exercises

Work in your local clone of [HFT Research Lab](../../README.md). **Predict first, then verify.** The existing tests should remain green.

## Exercise 1 — Read the order (5 min)

Inspect `include/hft/order.hpp` and `src/main.cpp`.

1. Identify which field represents the side of an order.
2. Which fields are unsigned integers?
3. Does `price_ticks = 4200000` by itself mean a specific price in PLN or USD? Why not?

**Done when:** You can explain every field without looking at the notes.

## Exercise 2 — Predict invalid input (5–10 min)

In `src/main.cpp`, temporarily change the example order from:

```cpp
const hft::Order order{1, hft::Side::buy, 4200000, 10};
```

to:

```cpp
const hft::Order order{1, hft::Side::buy, 4200000, 0};
```

Before building, answer:
- What does `is_valid(order)` return?
- Does the program print `BUY`?
- What exit code do you expect?

Build and verify:

```bash
cmake --build build -j 2
./build/hft_demo
echo $?
```

**Done when:** Your predictions match the output. Restore `quantity` to `10` and rebuild.

## Exercise 3 — Step into validation (5–10 min)

1. Set a breakpoint on `if (!hft::is_valid(order)) return 1;` in `src/main.cpp`.
2. Start **CMake: Debug** (use the command palette or your configured shortcut).
3. Press **F11** and find the implementation in `src/order.cpp`.
4. Inspect the three numeric fields in **Locals**.
5. Use **Shift+F11** to return to `main()`; continue with **F5**.

**Done when:** You can explain the difference between Step Over, Step Into, and Step Out.

## Exercise 4 — Test the boundary (10 min)

Open `tests/order_test.cpp`. Locate the existing assertions for zero ID, zero price, and zero quantity.

Add one more assertion checking that a **negative** `price_ticks` value is rejected:

```cpp
assert(!hft::is_valid(Order{7, Side::buy, -1, 1}));
```

Run:

```bash
cmake --build build -j 2
ctest --test-dir build --output-on-failure
```

**Done when:** All tests pass and you can explain why a signed price type can hold `-1` while the validator rejects it.

## Checkpoint

- [ ] I can explain `Order` and `Side`.
- [ ] I understand `&&` and `!`.
- [ ] I can distinguish exit code `0` from `1`.
- [ ] I can use F10, F11, Shift+F11, and F5 while debugging.
- [ ] I added a passing boundary test.

## Keep your work clean

If you only experimented and do not want to keep your changes, inspect them with `git diff` before restoring. Do **not** blindly discard uncommitted work.

To update the course materials later: `git pull` from the repository root.
