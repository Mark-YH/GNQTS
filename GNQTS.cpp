//
// Created by Mark Hsu on 2019/11/6.
//

#include "GNQTS.h"
#include <climits>

#define DEBUG 1

GNQTS::GNQTS() {
    this->model.init();
    this->model.setPopulation(10);
    this->model.setGeneration(1);
    this->model.setTheta(0.0004);
    this->model.setFund(10000000.0);
    this->model.setFee(0.001425);
    this->model.setTax(0.003);

    // allocate memory
    this->pMatrix = new double[this->model.getLength()];
    this->particle = new Particle[this->model.getPopulation()];
    for (int i = 0; i < this->model.getPopulation(); i++) {
        this->particle[i].setSolutionSize(this->model.getLength());
    }

    // initialize probability matrix
    for (int i = 0; i < this->model.getLength(); i++) {
        this->pMatrix[i] = 0.5;
    }

    // global best
    this->bestParticle.fitness = 0;
    bestGeneration = 0;

#if DEBUG
    Logger logger("../log/init.csv");
    for (int i = 0; i < this->model.getPopulation(); i++) {
        for (int j = 0; j < this->model.getLength(); j++) {
            logger.writeComma(this->particle[i].solution[j]);
        }
        logger.writeLine("");
    }
#endif
}

void GNQTS::run() {
    for (int i = 0; i < this->model.getGeneration(); i++) {
        measure();
        calcFitness(i);
        update();
        mutate();
    }
    Logger result("../log/result.csv");
    result.writeComma("Found best at");
    result.writeLine(this->bestGeneration);
    result.writeComma("Best portfolio:");
    for (int i = 0; i < this->model.getLength(); i++) {
        result.writeComma(this->bestParticle.solution[i]);
    }
    result.writeLine("");
}

void GNQTS::measure() {
    auto *randomMatrix = new double[this->model.getLength()];

    for (int i = 0; i < this->model.getPopulation(); i++) {
        // generate a random matrix
        for (int j = 0; j < this->model.getLength(); j++) {
            randomMatrix[j] = (double) rand() / RAND_MAX;
        }
#if DEBUG
        Logger logger("../log/random_matrix.csv", 2);
        logger.write("Individual ");
        logger.writeComma(i);
        for (int j = 0; j < this->model.getLength(); j++) {
            logger.writeComma(randomMatrix[j]);
        }
        logger.writeLine("");
#endif
        // measure the value of each item (taken or not)
        for (int j = 0; j < this->model.getLength(); j++) {
            if (this->pMatrix[j] > randomMatrix[j]) {
                this->particle[i].solution[j] = 1;
            } else {
                this->particle[i].solution[j] = 0;
            }
        }
    }

#if DEBUG
    Logger logger("../log/measure.csv");
    for (int i = 0; i < this->model.getPopulation(); i++) {
        for (int j = 0; j < this->model.getLength(); j++) {
            logger.writeComma(this->particle[i].solution[j]);
        }
        logger.writeLine("");
    }
#endif
}

void GNQTS::calcFitness(int gen) {
    // local worst
    this->worstParticle.fitness = INT_MAX;

    for (int i = 0; i < this->model.getPopulation(); i++) {
        this->particle[i].fitness = this->model.getFitness(this->particle[i], gen, i);
#if DEBUG
        Logger logger("../log/fitness.csv");
        logger.write("Individual ");
        logger.writeComma(i);
        logger.writeLine(this->particle[i].fitness);
#endif
        // Check if it needs to update best particle
        if (this->particle[i].fitness > this->bestParticle.fitness) {
            memcpy(&this->bestParticle, &this->particle[i], sizeof(bestParticle));
            bestGeneration = gen;
        }
        // Check if it needs to update worst particle
        if (this->worstParticle.fitness > this->particle[i].fitness) {
            memcpy(&this->worstParticle, &this->particle[i], sizeof(bestParticle));
        }
    }
#if DEBUG
    Logger logger("../log/best&worst_particle.csv");

    logger.writeLine("Best particle:");
    for (int j = 0; j < this->model.getLength(); j++) {
        logger.writeComma(this->bestParticle.solution[j]);
    }
    logger.writeLine("");
    logger.writeComma("Best fitness:");
    logger.writeLine(this->bestParticle.fitness);

    logger.writeLine("Worst particle:");
    for (int j = 0; j < this->model.getLength(); j++) {
        logger.writeComma(this->worstParticle.solution[j]);
    }
    logger.writeLine("");
    logger.writeComma("Worst fitness:");
    logger.writeLine(this->worstParticle.fitness);
#endif
}

void GNQTS::update() {
    for (int i = 0; i < this->model.getLength(); i++) {
        if (this->bestParticle.solution[i] == 1 && worstParticle.solution[i] == 0) {
            this->pMatrix[i] += this->model.getTheta();
        } else if (bestParticle.solution[i] == 0 && worstParticle.solution[i] == 1) {
            this->pMatrix[i] -= this->model.getTheta();
        }
    }
#if DEBUG
    Logger logger("../log/update.csv", 2);
    for (int i = 0; i < this->model.getLength(); i++) {
        logger.writeComma(pMatrix[i]);
    }
    logger.writeLine("");
#endif
}

void GNQTS::mutate() {
    for (int i = 0; i < this->model.getLength(); i++) {
        if (this->bestParticle.solution[i] == 1 && worstParticle.solution[i] == 0 && pMatrix[i] < 0.5) {
            this->pMatrix[i] = 1 - this->pMatrix[i] - this->model.getTheta();
        } else if (bestParticle.solution[i] == 0 && worstParticle.solution[i] == 1 && pMatrix[i] > 0.5) {
            this->pMatrix[i] = 1 - this->pMatrix[i] + this->model.getTheta();
        }
    }
#if DEBUG
    Logger logger("../log/mutate.csv", 2);
    for (int i = 0; i < this->model.getLength(); i++) {
        logger.writeComma(pMatrix[i]);
    }
    logger.writeLine("");
#endif
}
