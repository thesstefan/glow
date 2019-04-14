#include <iostream>
#include <cassert>
#include <cstdlib>

#include "kernel.h"

std::vector<float> get_random_vector(int max_value, int size) {
    std::vector<float> vec;

    while (vec.size() != size)
        vec.push_back(std::rand() % max_value);

    return vec;
}

void kernel_constructor_test(int test_cases = 10) {
    std::cerr << std::endl << "Kernel::Kernel(std::vector<std::vector<float>>) TESTS" << std::endl << std::endl;

    for (int test_index = 0; test_index < test_cases; test_index++) {
        std::vector<std::vector<float>> kernelData;

        int height = std::rand() % 50 + 1;
        int width = std::rand() % 50 + 1;
        int max_value = std::rand() % 100 + 1;

        for (int row = 0; row < height; row++)
            kernelData.push_back(get_random_vector(max_value, width));

        Kernel kernel(kernelData);

        assert(kernel.getWidth() == width);
        assert(kernel.getHeight() == height);

        for (int row = 0; row < kernel.getHeight(); row++)
            for (int col = 0; col < kernel.getWidth(); col++)
                assert(kernel[row][col] == kernelData[row][col]);

        std::cerr << "#" << test_index + 1 << " TEST CASE (WIDTH = " << width << ", HEIGHT = " << height << ", MAX_VALUE = " << max_value << ") PASSED" << std::endl;
    }
}

void kernel_normalize_test(int test_cases = 10) {
    std::srand(time(nullptr));

    std::cerr << std::endl << "Kernel::normalize TESTS" << std::endl << std::endl; 

    for (int test_index = 0; test_index < test_cases; test_index++) {
        float random_sigma = std::rand() / 3;

        Kernel kernel(random_sigma, 5, 5);
        kernel.normalize();

        float sum = 0;
        for (int row = 0; row < kernel.getHeight(); row++)
            for (const auto& col : kernel[row])
                sum += col;

        assert(sum >= 1 - 0.000001 && sum <= 1 + 0.000001);

        std::cerr << "#" << test_index + 1 << " TEST CASE (SIGMA = " << random_sigma << ") PASSED" << std::endl;
    }
}

void kernel_test() {
    kernel_normalize_test(20);
    kernel_constructor_test(20);
}

int main() {
    kernel_test();

    return 0;
}
