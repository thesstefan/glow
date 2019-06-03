#include <iostream>
#include <cassert>
#include <vector>

#include "kernel.h"

void print_kernel(const Kernel &kernel) {
    for (size_t row = 0; row < kernel.getHeight(); row++) {
        for (size_t col = 0; col < kernel.getWidth(); col++) 
            std::cout << kernel[row][col] << " ";

        std::cout << std::endl;
    }
}

bool is_kernel_equal(const Kernel &kernel_1, const Kernel &kernel_2) {
    if (kernel_1.getHeight() != kernel_2.getHeight() || kernel_1.getWidth() != kernel_2.getWidth())
        return false;

    for (size_t row = 0; row < kernel_1.getHeight(); row++)
        for (size_t col = 0; col < kernel_1.getWidth(); col++)
            if (std::abs(kernel_1[row][col] - kernel_2[row][col]) > 0.0001)
                return false;

    return true;
}

void gaussian_kernel_test() {
    size_t caseNumber = 0;
    std::cerr << "GAUSSIAN KERNEL TEST" << std::endl << std::endl;

    std::cout << "CASE #" << ++caseNumber << ": size:7x7, variance=0.707106789 -> ";
    
    GaussianKernel gauss_1(7, 7, 0.707106789);

    Kernel kernel_1(std::vector<std::vector<double>> {
                         std::vector<double>{0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
                         std::vector<double>{0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
                         std::vector<double>{0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
                         std::vector<double>{0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771},
                         std::vector<double>{0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
                         std::vector<double>{0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
                         std::vector<double>{0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067}
                   });

    assert(is_kernel_equal(kernel_1, gauss_1));

    std::cout << "PASSED" << std::endl;

    std::cout << std::endl << caseNumber << " TESTS PASSED SUCCESSFULLY" << std::endl;
}

void kernel_test() {
    gaussian_kernel_test();
}

int main() {
    kernel_test();

    return 0;
}
