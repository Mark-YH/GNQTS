//
// Created by Mark Hsu on 2019/11/19.
//

#ifndef GNQTS_STOCK_STOCK_H
#define GNQTS_STOCK_STOCK_H

#include <string>
#include <vector>

class Stock {
public:
    std::string code{};
    std::vector<double> price{};
    std::vector<double> fs{}; // fund standardization
    int length{};

    Stock() = delete;

    explicit Stock(int _numOfDay) : length{_numOfDay}, code{""} {
        for (int i = 0; i < _numOfDay; i++) {
            this->price.emplace_back(0);
            this->fs.emplace_back(0);
        }
    };
};


#endif //GNQTS_STOCK_STOCK_H
