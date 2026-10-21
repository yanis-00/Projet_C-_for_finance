
#include <iostream>
#include "CallOption.h"
#include "PutOption.h"

int main() {
    CallOption call(1.0, 100.0);
    PutOption put(1.0, 100.0);

    std::cout << "Call payoff: " << call.payoff(120.0) << std::endl;
    std::cout << "Put payoff: " << put.payoff(80.0) << std::endl;

    return 0;
}
