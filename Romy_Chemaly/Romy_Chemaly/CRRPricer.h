#pragma once

#include "Option.h"
#include "BinaryTree.h"

class CRRPricer {
private:
    Option* _option;
    int _depth;
    double _asset_price;
    double _up;
    double _down;
    double _interest_rate;
    BinaryTree<double> _tree;
    bool _computed;

public:
    CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate);
    void compute();
    double get(int n, int i) const;
    double operator()(bool closed_form = false);
};
