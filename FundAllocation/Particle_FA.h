//
// Created by Lab114 on 2020/3/27.
//

#ifndef GNQTS_STOCK_PARTICLE_FA_H
#define GNQTS_STOCK_PARTICLE_FA_H

class ParticleFA {
public:
    //TODO 把 solution 改成多維陣列 有幾檔就有幾個solution陣列
    int *solution; // ratio
    double fitness;
    int length;

    /** @param _length: the number of bits */
    explicit ParticleFA(int _length) {
        this->fitness = -DBL_MAX;
        this->length = _length;
        this->solution = new int[_length];
    }

    ParticleFA(const ParticleFA &copy) {
        if (this != &copy) {
            this->fitness = copy.fitness;
            this->length = copy.length;
            this->solution = new int[copy.length];
            for (int i = 0; i < length; i++)
                this->solution[i] = copy.solution[i];
        }
    }

    ParticleFA &operator=(const ParticleFA &copy) {
        if (this != &copy) {
            this->fitness = copy.fitness;
            this->length = copy.length;
            this->solution = new int[copy.length];
            for (int i = 0; i < length; i++)
                this->solution[i] = copy.solution[i];
        }
        return *this;
    }

    ~ParticleFA() {
        delete[] solution;
    }
};

#endif //GNQTS_STOCK_PARTICLE_FA_H
