

#include "BlackScholesPricer.h"
#include "EuropeanDigitalCallOption.h"
#include "EuropeanDigitalPutOption.h"
#include <cmath>
#include <stdexcept>

// Constructor for vanilla options
BlackScholesPricer::BlackScholesPricer(
    EuropeanVanillaOption* option,
    double asset_price,
    double interest_rate,
    double volatility)
    : _option(option),
    _digitalOption(nullptr),
    _asset_price(asset_price),
    _interest_rate(interest_rate),
    _volatility(volatility)
{
    if (option == nullptr || asset_price <= 0 ||
        volatility <= 0 || option->getExpiry() <= 0 ||
        option->_strike <= 0) {
        throw std::invalid_argument("Invalid pricing parameters");
    }
}

// Constructor for digital options
BlackScholesPricer::BlackScholesPricer(
    EuropeanDigitalOption* option,
    double asset_price,
    double interest_rate,
    double volatility)
    : _option(nullptr),
    _digitalOption(option),
    _asset_price(asset_price),
    _interest_rate(interest_rate),
    _volatility(volatility)
{
    if (option == nullptr || asset_price <= 0 ||
        volatility <= 0 || option->getExpiry() <= 0 ||
        option->_strike <= 0) {
        throw std::invalid_argument("Invalid pricing parameters");
    }
}

// Calculate option price
double BlackScholesPricer::operator()() {
    bool isDigital = (_digitalOption != nullptr);

    double S = _asset_price;
    double K = isDigital ? _digitalOption->_strike : _option->_strike;
    double T = isDigital ? _digitalOption->getExpiry() : _option->getExpiry();
    double r = _interest_rate;
    double sigma = _volatility;

    double d1 = (std::log(S / K) +
        (r + 0.5 * sigma * sigma) * T)
        / (sigma * std::sqrt(T));

    double d2 = d1 - sigma * std::sqrt(T);

    auto N = [](double x) {
        return 0.5 * std::erfc(-x / std::sqrt(2.0));
        };

    if (isDigital) {
        double discount = std::exp(-r * T);

        if (dynamic_cast<EuropeanDigitalCallOption*>(_digitalOption) != nullptr) {
            return discount * N(d2);
        }

        if (dynamic_cast<EuropeanDigitalPutOption*>(_digitalOption) != nullptr) {
            return discount * N(-d2);
        }

        throw std::invalid_argument("Unsupported digital option type");
    }

    if (_option->GetOptionType() == optionType::call) {
        return S * N(d1) - K * std::exp(-r * T) * N(d2);
    }

    return K * std::exp(-r * T) * N(-d2) - S * N(-d1);
}

// Calculate option Delta
double BlackScholesPricer::delta() {
    bool isDigital = (_digitalOption != nullptr);

    double S = _asset_price;
    double K = isDigital ? _digitalOption->_strike : _option->_strike;
    double T = isDigital ? _digitalOption->getExpiry() : _option->getExpiry();
    double r = _interest_rate;
    double sigma = _volatility;

    double d1 = (std::log(S / K) +
        (r + 0.5 * sigma * sigma) * T)
        / (sigma * std::sqrt(T));

    auto N = [](double x) {
        return 0.5 * std::erfc(-x / std::sqrt(2.0));
        };

    if (isDigital) {
        double d2 = d1 - sigma * std::sqrt(T);

        double phi = std::exp(-0.5 * d2 * d2)
            / std::sqrt(2.0 * std::acos(-1.0));

        double digitalDelta = std::exp(-r * T) * phi
            / (S * sigma * std::sqrt(T));

        if (dynamic_cast<EuropeanDigitalCallOption*>(_digitalOption) != nullptr) {
            return digitalDelta;
        }

        if (dynamic_cast<EuropeanDigitalPutOption*>(_digitalOption) != nullptr) {
            return -digitalDelta;
        }

        throw std::invalid_argument("Unsupported digital option type");
    }

    double N_d1 = N(d1);

    if (_option->GetOptionType() == optionType::call) {
        return N_d1;
    }

    return N_d1 - 1.0;
}
