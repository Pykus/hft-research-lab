#include "hft/order.hpp"
#include <string>
namespace hft {
bool is_valid(const Order& order) noexcept {
    return order.id > 0 && order.price_ticks > 0 && order.quantity > 0;
}
std::string describe(const Order& order) {
    return std::string(order.side == Side::buy ? "BUY" : "SELL") +
        " id=" + std::to_string(order.id) +
        " price_ticks=" + std::to_string(order.price_ticks) +
        " quantity=" + std::to_string(order.quantity);
}
} // namespace hft
