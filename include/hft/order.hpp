#pragma once
#include <cstdint>
#include <string>
namespace hft {
enum class Side { buy, sell };
struct Order {
    std::uint64_t id;
    Side side;
    std::int64_t price_ticks;
    std::uint64_t quantity;
};
bool is_valid(const Order& order) noexcept;
std::string describe(const Order& order);
} // namespace hft
