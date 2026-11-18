#include "CRRPricer.h"
#include <cmath>
#include <stdexcept>

CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate)
    : _option(option), _depth(depth), _asset_price(asset_price), _up(up), _down(down), _interest_rate(interest_rate), _computed(false) {

    // V?rification de la condition d'absence d'arbitrage : D < R < U
    if (_option == nullptr) {
        throw std::invalid_argument("Option pointer is null.");
    }
    if (_option->isAsianOption()) {
        throw std::invalid_argument("CRRPricer cannot price Asian options.");
    }
    if (!(_down < _interest_rate && _interest_rate < _up)) {
        throw std::invalid_argument("Arbitrage opportunity detected: must have D < R < U.");
    }
    if (_down <= -1.0 || _asset_price <= 0.0 || _depth < 0) {
        throw std::invalid_argument("Invalid model parameters.");
    }

    _tree.setDepth(_depth);
}

void CRRPricer::compute() {
    // 1. Calcul des payoffs ? maturit? (n = N)
    for (int i = 0; i <= _depth; ++i) {
        double s_ni = _asset_price * std::pow(1.0 + _up, i) * std::pow(1.0 + _down, _depth - i);
        _tree.setNode(_depth, i, _option->payoff(s_ni));
    }

    // 2. Induction r?trograde (Backward induction)
    double q = (_interest_rate - _down) / (_up - _down);
    double discount = 1.0 / (1.0 + _interest_rate);

    for (int n = _depth - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            double h_up = _tree.getNode(n + 1, i + 1);
            double h_down = _tree.getNode(n + 1, i);
            double h_ni = discount * (q * h_up + (1.0 - q) * h_down);
            _tree.setNode(n, i, h_ni);
        }
    }

    _computed = true;
}

double CRRPricer::get(int n, int i) const {
    return _tree.getNode(n, i);
}

double CRRPricer::operator()(bool closed_form) {
    if (closed_form) {
        double q = (_interest_rate - _down) / (_up - _down);
        double sum = 0.0;
        double logFact = std::lgamma(_depth + 1.0);

        // Binomial weights computed in log space to avoid overflow for large N
        for (int i = 0; i <= _depth; ++i) {
            double logWeight = logFact - std::lgamma(i + 1.0) - std::lgamma(_depth - i + 1.0)
                + i * std::log(q) + (_depth - i) * std::log(1.0 - q);
            double s_ni = _asset_price * std::pow(1.0 + _up, i) * std::pow(1.0 + _down, _depth - i);
            sum += std::exp(logWeight) * _option->payoff(s_ni);
        }
        return sum / std::pow(1.0 + _interest_rate, _depth);
    }

    if (!_computed) {
        compute();
    }
    return _tree.getNode(0, 0);
}