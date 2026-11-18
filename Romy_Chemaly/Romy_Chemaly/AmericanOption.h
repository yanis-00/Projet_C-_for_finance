#pragma once
#include "Option.h"

class AmericanOption : public Option {
public:
    AmericanOption(double expiry);
    bool isAmericanOption() override;
};
