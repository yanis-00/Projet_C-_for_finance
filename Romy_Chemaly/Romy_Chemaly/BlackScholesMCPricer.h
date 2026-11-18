#pragma once
#include "Option.h"
#include <vector>

class BlackScholesMCPricer {
private:
    Option* _option;
    double _initialPrice;
    double _interestRate;
    double _volatility;
    int _nbPaths;
    double _mean; // Moyenne courante des payoffs actualisés
    double _m2;   // Somme des carrés des écarts à la moyenne (pour la variance)

public:
    BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility);
    int getNbPaths();
    void generate(int nb_paths);
    double operator()();
    std::vector<double> confidenceInterval();
};