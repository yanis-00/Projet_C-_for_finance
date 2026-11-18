#include "AmericanCallOption.h"
#include <stdexcept>

AmericanCallOption::AmericanCallOption(double expiry, double strike)
    : AmericanOption(expiry), _strike(strike) {
    if (expiry < 0 || strike < 0)
        throw std::invalid_argument("expiry and strike must be nonnegative");
}

double AmericanCallOption::payoff(double z) {
    return z >= _strike ? z - _strike : 0.0;
}
