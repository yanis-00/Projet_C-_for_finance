#include "MT.h"

MT::MT() : _gen(std::random_device{}()) {}

MT& MT::getInstance() {
    static MT instance;
    return instance;
}

double MT::rand_unif() {
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(getInstance()._gen);
}

double MT::rand_norm() {
    static std::normal_distribution<double> dist(0.0, 1.0);
    return dist(getInstance()._gen);
}