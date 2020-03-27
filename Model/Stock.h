//
// Created by Mark Hsu on 2019/11/19.
//

#ifndef GNQTS_STOCK_STOCK_H
#define GNQTS_STOCK_STOCK_H

class Stock {
public:
    int code{};
    double *price{};
    double *fs{}; // fund standardization
    int length;

    Stock() = default;

    void setPriceSize(int size) {
        this->price = new double[size];
        this->fs = new double[size];
        this->length = size;
    }

    ~Stock() {
        delete[] this->price;
        delete[] this->fs;
    }

    Stock(const Stock &instance) {
        if (this != &instance) {
            this->code = instance.code;
            this->length = instance.length;
            this->price = new double[instance.length];
            this->fs = new double[instance.length];
            for (int i = 0; i < instance.length; i++) {
                this->price[i] = instance.price[i];
                this->fs[i] = instance.fs[i];
            }
        }
    }

    Stock &operator=(const Stock &instance) {
        if (this != &instance) {
            this->code = instance.code;
            this->length = instance.length;
            this->price = new double[instance.length];
            this->fs = new double[instance.length];
            for (int i = 0; i < instance.length; i++) {
                this->price[i] = instance.price[i];
                this->fs[i] = instance.fs[i];
            }
        }
        return *this;
    }
};


#endif //GNQTS_STOCK_STOCK_H
