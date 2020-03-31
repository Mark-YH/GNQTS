//
// Created by Lab114 on 2020/3/27.
//

#ifndef GNQTS_STOCK_PARTICLE_FA_H
#define GNQTS_STOCK_PARTICLE_FA_H

class ParticleFA {
public:
    int **solution; // ratio
    double fitness;
    int length;

    /** @param _length: the number of bits */
    explicit ParticleFA(int _length) {
        this->fitness = -DBL_MAX;
        this->length = _length;
        this->solution = new int *[_length];
        for (int i = 0; i < length; i++) {
            this->solution[i] = new int[24];
            for (int j = 0; j < 24; j++)
                this->solution[i][j] = 0;
        }
    }

    ParticleFA(const ParticleFA &copy) {
        if (this != &copy) {
            this->fitness = copy.fitness;
            this->length = copy.length;
            this->solution = new int *[copy.length];
            for (int i = 0; i < length; i++) {
                this->solution[i] = new int[24];
                for (int j = 0; j < 24; j++)
                    this->solution[i][j] = copy.solution[i][j];
            }
        }
    }

    ParticleFA &operator=(const ParticleFA &copy) {
        if (this != &copy) {
            this->fitness = copy.fitness;
            this->length = copy.length;
            this->solution = new int *[copy.length];
            for (int i = 0; i < length; i++) {
                this->solution[i] = new int[24];
                for (int j = 0; j < 24; j++)
                    this->solution[i][j] = copy.solution[i][j];
            }
        }
        return *this;
    }

    bool operator>(const ParticleFA &instance) {
        return this->fitness > instance.fitness;
    }

    bool operator>=(const ParticleFA &instance) {
        return this->fitness >= instance.fitness;
    }

    bool operator<(const ParticleFA &instance) {
        return this->fitness < instance.fitness;
    }

    bool operator<=(const ParticleFA &instance) {
        return this->fitness <= instance.fitness;
    }

    bool operator==(const ParticleFA &instance) {
        return this->fitness == instance.fitness;
    }

    ~ParticleFA() {
        for (int i = 0; i < length; i++)
            delete[] solution[i];
        delete[] solution;
    }
};

#endif //GNQTS_STOCK_PARTICLE_FA_H
