#include "AmericanPutOption.h"
#include <stdexcept>

AmericanPutOption::AmericanPutOption(double expiry, double strike)
    : AmericanOption(expiry), _strike(strike) {
    if (expiry < 0 || strike < 0)
        throw std::invalid_argument("expiry and strike must be nonnegative");
}

double AmericanPutOption::payoff(double z) {
    return _strike >= z ? _strike - z : 0.0;
}
