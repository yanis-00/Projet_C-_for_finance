#pragma once
#include "Option.h"
#include <vector>

class AsianOption : public Option {
private:
    std::vector<double> _timeSteps;

public:
    AsianOption(std::vector<double> timeSteps);
    std::vector<double> getTimeSteps();
    double payoffPath(std::vector<double> prices) override;
    bool isAsianOption() override;
};

class AsianCallOption : public AsianOption {
private:
    double _strike;

public:
    AsianCallOption(std::vector<double> timeSteps, double strike);
    double payoff(double z) override;
};

class AsianPutOption : public AsianOption {
private:
    double _strike;

public:
    AsianPutOption(std::vector<double> timeSteps, double strike);
    double payoff(double z) override;
};