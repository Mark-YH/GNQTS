//
// Created by Mark Hsu on 2019/11/19.
//

#ifndef GNQTS_STOCK_STOCK_H
#define GNQTS_STOCK_STOCK_H

class Stock {
public:
    int code;
    double *price;

    void setPriceSize(int size) {
        this->price = new double[size];
    }
};

#endif //GNQTS_STOCK_STOCK_H
