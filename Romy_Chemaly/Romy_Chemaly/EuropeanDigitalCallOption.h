
#ifndef EUROPEANDIGITALCALLOPTION_H
#define EUROPEANDIGITALCALLOPTION_H

#include "EuropeanDigitalOption.h"

class EuropeanDigitalCallOption : public EuropeanDigitalOption {
public:
    EuropeanDigitalCallOption(double expiry, double strike);

    double payoff(double z) override;
};

#endif
