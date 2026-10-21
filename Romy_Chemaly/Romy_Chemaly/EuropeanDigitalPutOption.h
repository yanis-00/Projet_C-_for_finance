
#ifndef EUROPEANDIGITALPUTOPTION_H
#define EUROPEANDIGITALPUTOPTION_H

#include "EuropeanDigitalOption.h"

class EuropeanDigitalPutOption : public EuropeanDigitalOption {
public:
    EuropeanDigitalPutOption(double expiry, double strike);

    double payoff(double z) override;
};

#endif
