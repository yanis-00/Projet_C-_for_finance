#pragma once
#include "AmericanOption.h"

class AmericanPutOption : public AmericanOption {
private:
    double _strike;
public:
    AmericanPutOption(double expiry, double strike);
    double payoff(double z) override;
};
