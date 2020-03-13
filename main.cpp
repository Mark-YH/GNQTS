#include "GNQTS.h"
#include <iostream>
#include <chrono>

int main() {
    std::remove("../log/result.csv");
    std::remove("../log/best&worst_particle.csv");
    std::remove("../log/fitness.csv");
    std::remove("../log/fund_standardization.csv");
    std::remove("../log/measure.csv");
    std::remove("../log/mutate.csv");
    std::remove("../log/random_matrix.csv");
    std::remove("../log/read_data_for_debug.csv");
    std::remove("../log/stock.csv");
    std::remove("../log/stock2.csv");
    std::remove("../log/trend_value.csv");
    std::remove("../log/update.csv");
    std::remove("../log/section_result.csv");
    auto start = std::chrono::steady_clock::now();
    srand(114);
    Model *model = new Model(10, 100, 0.0004, 10000000.0, 0.001425, 0.003);
    for (int i = 0; i < ROUND; i++) { // round
        for (int j = 0; j < 2; j++) { // section
            Result *result = new Result();
            model->setResult(result);
            GNQTS *qts = new GNQTS(model);
            qts->run(i, j);
            result->foundBestCount = 1;
            result->generateOutput();
            delete qts;
            delete result;
        }
    }
    delete model;
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
    return 0;
}