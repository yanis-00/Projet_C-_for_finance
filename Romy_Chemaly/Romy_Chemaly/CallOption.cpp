
#include "CallOption.h"

CallOption::CallOption(double expiry, double strike)
    : EuropeanVanillaOption(expiry, strike)
{
}

double CallOption::payoff(double z) {
    if (z >= _strike) {
        return z - _strike;
    }
    return 0.0;
}

optionType CallOption::GetOptionType() {
    return optionType::call;
}
