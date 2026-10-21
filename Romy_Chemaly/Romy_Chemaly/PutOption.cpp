
#include "PutOption.h"

PutOption::PutOption(double expiry, double strike)
    : EuropeanVanillaOption(expiry, strike)
{
}

double PutOption::payoff(double z) {
    if (z <= _strike) {
        return _strike - z;
    }
    return 0.0;
}

optionType PutOption::GetOptionType() {
    return optionType::put;
}
