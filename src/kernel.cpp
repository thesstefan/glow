#include <stdexcept>

#include "kernel.h"
#include "gauss_distribution.h"

Kernel::Kernel(const std::vector<std::vector<float>>& kernelData) {
    if (kernelData.size() == 0)
        throw std::invalid_argument("std::invalid_argument: Kernel::Kernel() -> matrix can't be empty.");

    const size_t width = kernelData[0].size();
    for (size_t rowIndex = 1; rowIndex < kernelData.size(); rowIndex++)
        if (kernelData[rowIndex].size() != width)
            throw std::invalid_argument("std::invalid_argument: Kernel::Kernel() -> matrix must have constant dimensions.");

    if (kernelData.size() % 2 || width % 2)
        throw std::invalid_argument("std::invalid_argument: Kernel::Kernel() -> matrix must have odd dimensions.");

    this->kernelData = kernelData;
}

Kernel::Kernel(const Kernel& kernel) {
    kernelData.reserve(kernel.getHeight());

    for (int row = 0; row < kernel.getHeight(); row++)
        kernelData.push_back(kernel[row]);
}

void Kernel::normalize() {
    float sum = 0;
    for (const auto &row : kernelData)
        for (const auto &element : row)
            sum += element;

    const float multiplier = 1 / sum;
    for (auto &row : kernelData)
        for (auto &element : row)
            element *= multiplier;
}

std::vector<float> Kernel::operator[](const size_t index) const {
    return kernelData.at(index);
}

int Kernel::getHeight() const {
    return kernelData.size();
}

int Kernel::getWidth() const {
    return kernelData[0].size();
}

std::vector<std::vector<float>> GaussianKernel::getGaussianData(const int width, 
                                                                const int height, 
                                                                const float variance) const {
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("std::invalid_argument: getGaussianKernel() -> kernel must have positive dimensions.");

    if (width % 2 || height % 2)
        throw std::invalid_argument("std::invalid_argument: getGaussianKernel() -> kernel must have odd dimensions.");

    std::vector<std::vector<float>> kernelData;

    const float center_x = width / 2;
    const float center_y = height / 2;

    for (int height_index = 0; height_index < height; height_index++) {
        std::vector<float> kernelRow;

        for (int width_index = 0; width_index < width; width_index++) {
            const float probability = gaussianProbabilty(std::abs(width_index - center_x), 0, variance) *
                                      gaussianProbabilty(std::abs(height_index - center_y), 0, variance);

            kernelRow.push_back(probability);
        }

        kernelData.push_back(kernelRow);
    }

    return kernelData;
}

GaussianKernel::GaussianKernel(const int width, const int height, const float variance) :
    Kernel(getGaussianData(width, height, variance)) {}
