#include "GNQTS.h"
#include <iostream>
#include <chrono>

int main() {
    auto start = std::chrono::steady_clock::now();
    srand(114);
    GNQTS qts;
    qts.run();
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
    return 0;
}