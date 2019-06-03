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
            if (std::abs(kernel_1[row][col] - kernel_2[row][col]) > 0.0001) { 
                std::cout << kernel_1[row][col] << " " << kernel_2[row][col] << std::endl;

                return false;
            }

    return true;
}

void gaussian_kernel_test() {
    size_t caseNumber = 0;
    std::cout << "GAUSSIAN KERNEL TEST" << std::endl << std::endl;

    std::cout << "CASE #" << ++caseNumber << ": size:7x7, variance=0.707106789 -> ";
    
    GaussianKernel gauss_1(7, 7, 0.707106789);

    Kernel kernel_1(std::vector<std::vector<double>> {
                         std::vector<double> {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
                         std::vector<double> {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
                         std::vector<double> {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
                         std::vector<double> {0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771},
                         std::vector<double> {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
                         std::vector<double> {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
                         std::vector<double> {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067}
                   });

    assert(is_kernel_equal(kernel_1, gauss_1));

    std::cout << "PASSED" << std::endl;

    std::cout << std::endl << caseNumber << " TESTS PASSED SUCCESSFULLY" << std::endl << std::endl;
}

void kernel_normalize_test() {
    size_t caseNumber = 0;

    std::cout << "Kernel::normalize() TEST" << std::endl << std::endl;

    std::cout << "CASE #" << ++caseNumber << " -> ";

    Kernel kernel_1(std::vector<std::vector<double>> {
                        std::vector<double> {1, 1, 1},
                        std::vector<double> {1, 1, 1},
                        std::vector<double> {1, 1, 1}
                    });

    kernel_1.normalize();

    Kernel normalized_1(std::vector<std::vector<double>> { 
                            std::vector<double> {1./9, 1./9, 1./9},
                            std::vector<double> {1./9, 1./9, 1./9},
                            std::vector<double> {1./9, 1./9, 1./9}
                        });

    assert(is_kernel_equal(kernel_1, normalized_1));

    std::cout << "PASSED" << std::endl;

    std::cout << "CASE #" << ++caseNumber << " -> ";

    Kernel kernel_2(std::vector<std::vector<double>> {
                        std::vector<double> {1, 2, 1},
                        std::vector<double> {2, 4, 2},
                        std::vector<double> {1, 2, 1}
                    });

    kernel_2.normalize();

    Kernel normalized_2(std::vector<std::vector<double>> { 
                            std::vector<double> {1./16, 2./16, 1./16},
                            std::vector<double> {2./16, 4./16, 2./16},
                            std::vector<double> {1./16, 2./16, 1./16}
                        });

    assert(is_kernel_equal(kernel_2, normalized_2));

    std::cout << "PASSED" << std::endl;

    std::cout << "CASE #" << ++caseNumber << " -> ";

    Kernel kernel_3(std::vector<std::vector<double>> {
                        std::vector<double> {1, 2, 4,  2, 1},
                        std::vector<double> {2, 4, 8,  4, 2},
                        std::vector<double> {4, 8, 16, 8, 4},
                        std::vector<double> {2, 4, 8,  4, 2},
                        std::vector<double> {1, 2, 4,  2, 1}
                    });

    kernel_3.normalize();

    Kernel normalized_3(std::vector<std::vector<double>> {
                            std::vector<double> {1./100, 2./100, 4./100,  2./100, 1./100},
                            std::vector<double> {2./100, 4./100, 8./100,  4./100, 2./100},
                            std::vector<double> {4./100, 8./100, 16./100, 8./100, 4./100},
                            std::vector<double> {2./100, 4./100, 8./100,  4./100, 2./100},
                            std::vector<double> {1./100, 2./100, 4./100,  2./100, 1./100}
                       });

    assert(is_kernel_equal(kernel_3, normalized_3));

    std::cout << "PASSED" << std::endl;

    std::cout << std::endl << caseNumber << " TESTS PASSED SUCCESSFULLY" << std::endl;

}

void kernel_test() {
    gaussian_kernel_test();
    kernel_normalize_test();
}

int main() {
    kernel_test();

    return 0;
}
