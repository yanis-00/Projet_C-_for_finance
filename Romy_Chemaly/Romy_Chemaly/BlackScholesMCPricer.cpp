#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"
#include <cmath>
#include <stdexcept>

BlackScholesMCPricer::BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility)
    : _option(option),
    _initialPrice(initial_price),
    _interestRate(interest_rate),
    _volatility(volatility),
    _nbPaths(0),
    _mean(0.0),
    _m2(0.0) {
    if (_option == nullptr) {
        throw std::invalid_argument("Option pointer cannot be null.");
    }
}

int BlackScholesMCPricer::getNbPaths() {
    return _nbPaths;
}

void BlackScholesMCPricer::generate(int nb_paths) {
    if (nb_paths <= 0) {
        throw std::invalid_argument("Number of paths must be strictly positive.");
    }

    double T = _option->getExpiry();
    double discountFactor = std::exp(-_interestRate * T);
    double drift = _interestRate - 0.5 * _volatility * _volatility;

    bool isAsian = _option->isAsianOption();
    std::vector<double> timeSteps;
    if (isAsian) {
        timeSteps = static_cast<AsianOption*>(_option)->getTimeSteps();
    }
    else {
        // Pour une option européenne, m = 1 et t_1 = T
        timeSteps = { T };
    }

    std::vector<double> path(timeSteps.size());

    for (int i = 0; i < nb_paths; ++i) {
        double currentS = _initialPrice;
        double prevT = 0.0;

        for (size_t k = 0; k < timeSteps.size(); ++k) {
            double dt = timeSteps[k] - prevT;
            double z = MT::rand_norm();
            currentS *= std::exp(drift * dt + _volatility * std::sqrt(dt) * z);
            path[k] = currentS;
            prevT = timeSteps[k];
        }

        double discountedPayoff = discountFactor * _option->payoffPath(path);

        // Mise à jour incrémentale de la moyenne et de la variance (Welford)
        _nbPaths++;
        double delta = discountedPayoff - _mean;
        _mean += delta / static_cast<double>(_nbPaths);
        double delta2 = discountedPayoff - _mean;
        _m2 += delta * delta2;
    }
}

double BlackScholesMCPricer::operator()() {
    if (_nbPaths == 0) {
        throw std::runtime_error("Price estimate is undefined: no paths generated yet.");
    }
    return _mean;
}

std::vector<double> BlackScholesMCPricer::confidenceInterval() {
    if (_nbPaths < 2) {
        throw std::runtime_error("Not enough paths generated to compute a confidence interval.");
    }
    double variance = _m2 / static_cast<double>(_nbPaths - 1);
    double stdError = std::sqrt(variance / static_cast<double>(_nbPaths));
    double margin = 1.96 * stdError;

    return { _mean - margin, _mean + margin };
}