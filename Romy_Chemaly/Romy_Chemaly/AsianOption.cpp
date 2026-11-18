#include "AsianOption.h"
#include <numeric>
#include <stdexcept>

// --- AsianOption ---

AsianOption::AsianOption(std::vector<double> timeSteps)
    : Option(timeSteps.empty() ? 0.0 : timeSteps.back()), _timeSteps(timeSteps) {
    if (timeSteps.empty()) {
        throw std::invalid_argument("Time steps vector cannot be empty.");
    }
}

std::vector<double> AsianOption::getTimeSteps() {
    return _timeSteps;
}

double AsianOption::payoffPath(std::vector<double> prices) {
    double sum = std::accumulate(prices.begin(), prices.end(), 0.0);
    double mean = sum / static_cast<double>(prices.size());
    return payoff(mean);
}

bool AsianOption::isAsianOption() {
    return true;
}

// --- AsianCallOption ---

AsianCallOption::AsianCallOption(std::vector<double> timeSteps, double strike)
    : AsianOption(timeSteps), _strike(strike) {
    if (strike < 0.0) {
        throw std::invalid_argument("Strike cannot be negative.");
    }
}

double AsianCallOption::payoff(double z) {
    return (z >= _strike) ? (z - _strike) : 0.0;
}

// --- AsianPutOption ---

AsianPutOption::AsianPutOption(std::vector<double> timeSteps, double strike)
    : AsianOption(timeSteps), _strike(strike) {
    if (strike < 0.0) {
        throw std::invalid_argument("Strike cannot be negative.");
    }
}

double AsianPutOption::payoff(double z) {
    return (_strike >= z) ? (_strike - z) : 0.0;
}