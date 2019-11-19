//
// Created by Mark Hsu on 2019/11/13.
//

#ifndef GNQTS_STOCK_MODEL_H
#define GNQTS_STOCK_MODEL_H

class Model {
public:
    Model() = default;

    ~Model() = default;

    void setGeneration(int generation);

    int getGeneration();

    void setPopulation(int population);

    int getPopulation();

    void setTheta(double theta);

    double getTheta();

    void setFund(int fund);

    double getFund();

    void setLength(int length);

    int getLength();

private:
    int population;
    int generation;
    int length;
    double fund;
    double theta;
};

#endif //GNQTS_STOCK_MODEL_H
