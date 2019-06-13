#include <cassert>
#include <iostream>

#include "image_convolve.h"
#include "ofImage.h"

bool is_image_equal(const ofImage &image_1, const ofImage &image_2) {
    auto pixels_1 = image_1.getPixels();
    auto pixels_2 = image_2.getPixels();

    if (pixels_1.getWidth() != pixels_2.getWidth() || 
        image_1.getWidth() != image_2.getWidth() ||
        image_1.getHeight() != image_2.getHeight())
        return false;

    for (size_t pixelIndex = 0; pixelIndex < pixels_1.getWidth(); pixelIndex++)
        if (pixels_1.getColor(pixelIndex) != pixels_2.getColor(pixelIndex))
            return false;

    return true;
}

void convolve_test() {
    std::cout << "convolve() TEST" << std::endl << std::endl;

    size_t caseNumber = 0;

    ofImage image;
    image.load("test_img.bmp");

    std::cout << "CASE #" << ++caseNumber << " -> ";

    Kernel kernel_1(std::vector<std::vector<double>> {
                    std::vector<double> {0, 0, 0},
                    std::vector<double> {0, 1, 0},
                    std::vector<double> {0, 0, 0}
                  });

    kernel_1.normalize();
    
    ofImage output_1 = convolve(image, kernel_1);

    ofImage expected_1;
    expected_1.load("expected_1.bmp");

    if (is_image_equal(expected_1, output_1) == false) {
        std::cout << "FAILED" << std::endl;

        return;
    }

    std::cout << "PASSED" << std::endl;

    std::cout << "CASE #" << ++caseNumber << " -> ";

    Kernel kernel_2(std::vector<std::vector<double>> {
                    std::vector<double> {1, 2, 1},
                    std::vector<double> {2, 4, 2},
                    std::vector<double> {1, 2, 1}
                  });

    kernel_2.normalize();
    
    ofImage output_2 = convolve(image, kernel_2);

    ofImage expected_2;
    expected_2.load("expected_2.bmp");

    if (is_image_equal(expected_2, output_2) == false) {
        std::cout << "FAILED" << std::endl;

        return;
    }

    std::cout << "PASSED" << std::endl;

    std::cout << "CASE #" << ++caseNumber << " -> ";

    Kernel kernel_3(std::vector<std::vector<double>> {
                    std::vector<double> {4, 4, 4},
                    std::vector<double> {4, 20, 4},
                    std::vector<double> {4, 4, 4}
                  });

    kernel_3.normalize();
    
    ofImage output_3 = convolve(image, kernel_3);

    ofImage expected_3;
    expected_3.load("expected_3.bmp");

    if (is_image_equal(expected_3, output_3) == false) {
        std::cout << "FAILED" << std::endl;

        return;
    }

    std::cout << "PASSED" << std::endl;

    std::cout << std::endl << caseNumber << " TESTS PASSED SUCCESSFULLY" << std::endl << std::endl;
}

int main() {
    convolve_test();

    return 0;
}
