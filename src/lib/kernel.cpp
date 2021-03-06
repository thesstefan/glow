#include <stdexcept>
#include <iostream>

#include "kernel.h"
#include "gauss_distribution.h"

Kernel::Kernel(const std::vector<std::vector<double>>& kernelData) {
    if (kernelData.size() == 0)
        throw std::invalid_argument("std::invalid_argument: Kernel::Kernel() -> matrix can't be empty.");

    const size_t width = kernelData[0].size();
    for (size_t rowIndex = 1; rowIndex < kernelData.size(); rowIndex++)
        if (kernelData[rowIndex].size() != width)
            throw std::invalid_argument("std::invalid_argument: Kernel::Kernel() -> matrix must have constant dimensions.");

    if (kernelData.size() % 2 == 0 || width % 2 == 0)
        throw std::invalid_argument("std::invalid_argument: Kernel::Kernel() -> matrix must have odd dimensions.");

    this->kernelData = kernelData;
}

Kernel::Kernel(const Kernel& kernel) {
    kernelData.reserve(kernel.getHeight());

    for (size_t row = 0; row < kernel.getHeight(); row++)
        kernelData.push_back(kernel[row]);
}

double Kernel::normalize() {
    double sum = 0;
    for (const auto &row : kernelData)
        for (const auto &element : row)
            sum += element;

    const double multiplier = 1 / sum;
    for (auto &row : kernelData)
        for (auto &element : row)
            element *= multiplier;

    return multiplier;
}

std::vector<double> Kernel::operator[](const size_t index) const {
    return kernelData.at(index);
}

size_t Kernel::getHeight() const {
    return kernelData.size();
}

size_t Kernel::getWidth() const {
    return kernelData[0].size();
}

std::vector<std::vector<double>> GaussianKernel::getGaussianData(const size_t width, 
                                                                 const size_t height, 
                                                                 const double variance) const {
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("std::invalid_argument: getGaussianKernel() -> kernel must have positive dimensions.");

    if (width % 2 == 0 || height % 2 == 0)
        throw std::invalid_argument("std::invalid_argument: getGaussianKernel() -> kernel must have odd dimensions.");

    const size_t center_x = width / 2;
    const size_t center_y = height / 2;

    std::vector<std::vector<double>> kernelData;
    kernelData.reserve(height);

    for (size_t height_index = 0; height_index < height; height_index++) {
        std::vector<double> kernelRow;
        kernelRow.reserve(width);

        for (size_t width_index = 0; width_index < width; width_index++) {
            const double probability = gaussianProbabilty(std::abs(static_cast<int>(width_index - center_x)), 0, variance) *
                                       gaussianProbabilty(std::abs(static_cast<int>(height_index - center_y)), 0, variance);

            kernelRow.push_back(probability);
        }

        kernelData.push_back(kernelRow);
    }

    return kernelData;
}

GaussianKernel::GaussianKernel(const size_t width, const size_t height, const double variance) :
    Kernel(getGaussianData(width, height, variance)) {}
