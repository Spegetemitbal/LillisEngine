//
// Created by Somed on 1/17/2025.
//

#ifndef LILRANDOM_H
#define LILRANDOM_H

#include <random>
#include <time.h>

class LilRandom {
public:
    static int randInt(int minInclusive, int maxExclusive);
    static float randFloat(float minInclusive, float maxExclusive);
    template<typename T>
    static void shuffleVector(std::vector<T> toShuffle)
    {
        std::shuffle(toShuffle.begin(), toShuffle.end(), gen);
    }
private:
    static void GenerateSeed();
    static inline std::mt19937 gen;
    static inline bool initialized = false;
};



#endif //LILRANDOM_H
