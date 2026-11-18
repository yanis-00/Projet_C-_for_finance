
#include "EuropeanDigitalPutOption.h"

EuropeanDigitalPutOption::EuropeanDigitalPutOption(double expiry, double strike)
    : EuropeanDigitalOption(expiry, strike)
{
}

double EuropeanDigitalPutOption::payoff(double z) {
    if (z <= _strike) {
        return 1.0;
    }

    return 0.0;
}
