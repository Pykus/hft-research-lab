#include "hft/order.hpp"
#include <cassert>
int main() {
    using hft::Order;
    using hft::Side;
    assert(hft::is_valid(Order{1, Side::buy, 100, 5}));
    assert(!hft::is_valid(Order{0, Side::buy, 100, 5}));
    assert(!hft::is_valid(Order{1, Side::sell, 0, 5}));
    assert(!hft::is_valid(Order{1, Side::sell, 100, 0}));
    assert(hft::describe(Order{2, Side::sell, 150, 3}).find("SELL") != std::string::npos);
}
