#include <cassert>
#include <iostream>

#include "gauss_distribution.h"

bool aproximately_equal(const float nr_1, const float nr_2) {
    return std::abs(nr_1 - nr_2) < 0.00001;
}

void gauss_probability_test() {
    size_t caseNumber = 1;

    std::cout << "gaussianProbabilty() TEST" << std::endl << std::endl;

    std::cout << "CASE #" << caseNumber << ": x=1, mean=0, variance=1 -> ";

    caseNumber++;
    assert(aproximately_equal(gaussianProbabilty(1, 0, 1), 0.24197072));

    std::cout << "PASSED" << std::endl;

    std::cout << "CASE #" << caseNumber << ": x=-1, mean=0, variance=1 -> ";

    caseNumber++;
    assert(aproximately_equal(gaussianProbabilty(-1, 0, 1), 0.24197072));

    std::cout << "PASSED" << std::endl;

    std::cout << "CASE #" << caseNumber << ": x=1000, mean=900, variance=2000 -> ";

    caseNumber++;
    assert(aproximately_equal(gaussianProbabilty(1000, 900, 2000), 0.0007322));

    std::cout << "PASSED" << std::endl;

    std::cout << "CASE #" << caseNumber << ": x=2, mean=0, variance=0.5 -> ";

    caseNumber++;
    assert(aproximately_equal(gaussianProbabilty(2, 0, 0.5), 0.0103335));

    std::cout << "PASSED" << std::endl;

    std::cout << "CASE #" << caseNumber << ": x=2, mean=0, variance=2 -> ";

    caseNumber++;
    assert(aproximately_equal(gaussianProbabilty(2, 0, 2), 0.10378));

    std::cout << "PASSED" << std::endl;
    
    std::cout << "CASE #" << caseNumber << ": x=90, mean=100, variance=10 -> ";

    caseNumber++;
    assert(aproximately_equal(gaussianProbabilty(90, 100, 10), 0.00085));

    std::cout << "PASSED" << std::endl;

    std::cout << "CASE #" << caseNumber << ": x=-30, mean=-50, variance=40 -> ";

    caseNumber++;
    assert(aproximately_equal(gaussianProbabilty(-30, -50, 40), 0.0004250));

    std::cout << "PASSED" << std::endl;

    std::cout << std::endl << caseNumber - 1 << " TESTS PASSED SUCCESSFULLY" << std::endl;
}

int main() {
    gauss_probability_test();

    return 0;
}
