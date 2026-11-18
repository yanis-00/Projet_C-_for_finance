#pragma once

#ifndef EUROPEANDIGITALOPTION_H
#define EUROPEANDIGITALOPTION_H

#include "Option.h"

class EuropeanDigitalOption : public Option {
private:
    double _strike;

    friend class EuropeanDigitalCallOption;
    friend class EuropeanDigitalPutOption;
    friend class BlackScholesPricer;

public:
    EuropeanDigitalOption(double expiry, double strike);
};

#endif

