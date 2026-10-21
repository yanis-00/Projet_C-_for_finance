
#ifndef BLACKSCHOLESPRICER_H
#define BLACKSCHOLESPRICER_H

#include "EuropeanVanillaOption.h"
#include "EuropeanDigitalOption.h"

class BlackScholesPricer {
private:
    EuropeanVanillaOption* _option;
    EuropeanDigitalOption* _digitalOption;

    double _asset_price;
    double _interest_rate;
    double _volatility;

public:
    BlackScholesPricer(EuropeanVanillaOption* option,
        double asset_price,
        double interest_rate,
        double volatility);

    BlackScholesPricer(EuropeanDigitalOption* option,
        double asset_price,
        double interest_rate,
        double volatility);

    double operator()();

    double delta();
};

#endif

