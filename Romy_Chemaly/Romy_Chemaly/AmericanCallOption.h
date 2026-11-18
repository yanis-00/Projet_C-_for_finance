#pragma once
#include "AmericanOption.h"

class AmericanCallOption : public AmericanOption {
private:
    double _strike;
public:
    AmericanCallOption(double expiry, double strike);
    double payoff(double z) override;
};
