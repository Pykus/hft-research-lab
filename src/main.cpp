#include "hft/order.hpp"
#include <iostream>
int main() {
    const hft::Order order{1, hft::Side::buy, 4200000, 10};
    if (!hft::is_valid(order)) return 1;
    std::cout << "HFT Research Lab v0.1\n" << hft::describe(order) << '\n';
    return 0;
}
