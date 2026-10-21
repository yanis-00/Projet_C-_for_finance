
#ifndef OPTION_H
#define OPTION_H

    class Option {
    private:
        double _expiry;

    public:
        Option(double expiry);

        double getExpiry();

        virtual double payoff(double) = 0;
    };

#endif

    

