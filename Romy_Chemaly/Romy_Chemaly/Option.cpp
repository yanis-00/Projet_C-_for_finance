#include "Option.h"

// ... (constructeur et getExpiry déjà codés en Partie I) ...

double Option::payoffPath(std::vector<double> prices) {
    // Retourne h(S_tm), soit le payoff évalué sur le dernier prix de la trajectoire
    return payoff(prices.back());
}

bool Option::isAsianOption() {
    return false;
}