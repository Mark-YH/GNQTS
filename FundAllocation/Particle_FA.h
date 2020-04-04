//
// Created by Mark Hsu on 2020/3/27.
//

#ifndef GNQTS_STOCK_PARTICLE_FA_H
#define GNQTS_STOCK_PARTICLE_FA_H

#include <cfloat>

class ParticleFA {
public:
    vector<vector<int>> solution; // ratio
    double fitness;
    int numOfBit;
    int numOfChosen;

    /** @param _numOfBit: the number of bits
     *  @param _numOfChosen: the number of stocks */
    explicit ParticleFA(int _numOfBit, int _numOfChosen) :
            numOfBit{_numOfBit},
            numOfChosen{_numOfChosen},
            solution(_numOfChosen),
            fitness{-DBL_MAX} {
        for (int i = 0; i < _numOfChosen; i++) {
            for (int j = 0; j < _numOfBit; j++)
                this->solution[i].push_back(0);
        }
    }

    bool operator>(const ParticleFA &instance) const {
        return this->fitness > instance.fitness;
    }

    bool operator>=(const ParticleFA &instance) const {
        return this->fitness >= instance.fitness;
    }

    bool operator<(const ParticleFA &instance) const {
        return this->fitness < instance.fitness;
    }

    bool operator<=(const ParticleFA &instance) const {
        return this->fitness <= instance.fitness;
    }

    bool operator==(const ParticleFA &instance) const {
        return this->fitness == instance.fitness;
    }
};

#endif //GNQTS_STOCK_PARTICLE_FA_H
