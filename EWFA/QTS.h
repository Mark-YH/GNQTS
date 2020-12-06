//
// Created by Mark Hsu on 2019/11/6.
//

#ifndef PORTFOLIO_OPTIMIZATION_EWFA_QTS_H
#define PORTFOLIO_OPTIMIZATION_EWFA_QTS_H

#include "../Model/Model.h"
#include "Particle.h"

namespace EWFA {
    class QTS {
    public:
        QTS() = delete;

        explicit QTS(Model *m);

        ~QTS();

        void run();

        void measure();

        void evaluate(int generation);

        void update();

        int getBestGeneration() const;

    private:
        Model *model;
        vector<Particle> particle;
        Particle *bestParticle;
        Particle *worstParticle;
        int bestGeneration;
        double *pMatrix;
    };
}
#endif //PORTFOLIO_OPTIMIZATION_EWFA_QTS_H
