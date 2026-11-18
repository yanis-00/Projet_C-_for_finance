#pragma once

#ifndef OPTION_H
#define OPTION_H

#include <vector>

class Option {
private:
    double _expiry;

public:
    Option(double expiry);
    virtual ~Option() = default;

    double getExpiry();

    virtual double payoff(double) = 0;

    // Hooks used by Parts III and IV (default behaviour for European options)
    virtual double payoffPath(std::vector<double> path);
    virtual bool isAsianOption();
    virtual bool isAmericanOption();
};

#endif