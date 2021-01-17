//
// Created by Mark Hsu on 2020/3/27.
//

#ifndef PORTFOLIO_OPTIMIZATION_FA_QTS_H
#define PORTFOLIO_OPTIMIZATION_FA_QTS_H

#include "../Model/Model.h"
#include "ParticleFA.h"

namespace FA {
    class QTS {
    public:
        QTS() = delete;

        explicit QTS(Model &m, vector<int> &selection);

        ~QTS();

        vector<double> run(string &market, string &alias, string &mode, string &sw, string &periodName, int round);

        int getBestGeneration() const;

    private:
        void measure();

        void evaluate(int generation);

        void update();

        void normalize(vector<double> &_allocRatio) const;

        int numOfBit;
        Model *model;
        vector<int> stockSelection;
        vector<int> indexOfChosen;
        vector<ParticleFA> particle;
        ParticleFA *gBest;
        ParticleFA *lWorst;
        double **pMatrix;
        int bestGeneration;
    };
}
#endif //PORTFOLIO_OPTIMIZATION_FA_QTS_H
