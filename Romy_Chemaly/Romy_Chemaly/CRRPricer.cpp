#include "CRRPricer.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate)
    : _option(option), _depth(depth), _asset_price(asset_price), _up(up), _down(down), _interest_rate(interest_rate), _computed(false) {
    initialize();
}

// Constructeur avec les paramètres de Black-Scholes : U, D, R sont déduits de r, sigma et h = T / N
CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double r, double volatility)
    : _option(option), _depth(depth), _asset_price(asset_price), _up(0.0), _down(0.0), _interest_rate(0.0), _computed(false) {

    if (_depth <= 0 || volatility < 0.0) {
        throw std::invalid_argument("Invalid model parameters.");
    }

    double h = _option->getExpiry() / _depth;
    _up = std::exp((r + 0.5 * volatility * volatility) * h + volatility * std::sqrt(h)) - 1.0;
    _down = std::exp((r + 0.5 * volatility * volatility) * h - volatility * std::sqrt(h)) - 1.0;
    _interest_rate = std::exp(r * h) - 1.0;

    initialize();
}

void CRRPricer::initialize() {
    // Vérification de la condition d'absence d'arbitrage : D < R < U
    if (!(_down < _interest_rate && _interest_rate < _up)) {
        throw std::invalid_argument("Arbitrage opportunity detected: must have D < R < U.");
    }
    if (_down <= -1.0 || _asset_price <= 0.0 || _depth < 0) {
        throw std::invalid_argument("Invalid model parameters.");
    }

    _tree.setDepth(_depth);
    _exercise.setDepth(_depth);
}

void CRRPricer::compute() {
    bool american = _option->isAmericanOption();

    // 1. Calcul des payoffs à maturité (n = N)
    for (int i = 0; i <= _depth; ++i) {
        double s_ni = _asset_price * std::pow(1.0 + _up, i) * std::pow(1.0 + _down, _depth - i);
        _tree.setNode(_depth, i, _option->payoff(s_ni));
        _exercise.setNode(_depth, i, american);
    }

    // 2. Induction rétrograde (Backward induction)
    double q = (_interest_rate - _down) / (_up - _down);
    double discount = 1.0 / (1.0 + _interest_rate);

    for (int n = _depth - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            double h_up = _tree.getNode(n + 1, i + 1);
            double h_down = _tree.getNode(n + 1, i);
            double continuation = discount * (q * h_up + (1.0 - q) * h_down);

            if (american) {
                double s_ni = _asset_price * std::pow(1.0 + _up, i) * std::pow(1.0 + _down, n - i);
                double intrinsic = _option->payoff(s_ni);
                _tree.setNode(n, i, std::max(continuation, intrinsic));
                _exercise.setNode(n, i, intrinsic >= continuation);
            }
            else {
                _tree.setNode(n, i, continuation);
            }
        }
    }

    _computed = true;
}

double CRRPricer::get(int n, int i) const {
    return _tree.getNode(n, i);
}

bool CRRPricer::getExercise(int n, int i) const {
    return _exercise.getNode(n, i);
}

double CRRPricer::operator()(bool closed_form) {
    if (closed_form) {
        if (_option->isAmericanOption()) {
            throw std::invalid_argument("Closed form formula is not available for American options.");
        }

        double q = (_interest_rate - _down) / (_up - _down);
        double sum = 0.0;
        double comb = 1.0; // Coefficient binomial N! / (i! * (N - i)!) calculé itérativement

        for (int i = 0; i <= _depth; ++i) {
            if (i > 0) {
                comb = comb * (_depth - i + 1) / i;
            }
            double s_ni = _asset_price * std::pow(1.0 + _up, i) * std::pow(1.0 + _down, _depth - i);
            sum += comb * std::pow(q, i) * std::pow(1.0 - q, _depth - i) * _option->payoff(s_ni);
        }
        return sum / std::pow(1.0 + _interest_rate, _depth);
    }

    if (!_computed) {
        compute();
    }
    return _tree.getNode(0, 0);
}
