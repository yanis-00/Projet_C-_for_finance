#include <vector>

class Option {
private:
    double expiry;

public:
    Option(double expiry);
    virtual ~Option() = default;
    double getExpiry();
    virtual double payoff(double z) = 0;

    // --- Ajouts Partie III ---
    virtual double payoffPath(std::vector<double> prices);
    virtual bool isAsianOption();
};