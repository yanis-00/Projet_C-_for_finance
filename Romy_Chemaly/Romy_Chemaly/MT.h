#pragma once
#include <random>

class MT {
private:
    std::mt19937 _gen;

    // Constructeur privé et suppression de la copie (Singleton)
    MT();
    MT(const MT&) = delete;
    MT& operator=(const MT&) = delete;

    static MT& getInstance();

public:
    static double rand_unif();
    static double rand_norm();
};