#pragma once

#include <vector>
#include <stdexcept>

#include "gauss_distribution.h"

/** @brief Small matrix used for image processing. **/
class Kernel {
    private:
        /** @brief The values of the Kernel. **/
        std::vector<std::vector<float>> kernelData;

    public:
        /** @brief Constructs the Kernel. **/
        Kernel(const std::vector<std::vector<float>>& kernelData);

        /** @brief Copy-construct the Kernel. **/
        Kernel(const Kernel& kernel);

        /** @brief Normalizes the Kernel values. **/
        void normalize();

        /** @brief Returns the specified row of the Kernel. **/
        std::vector<float> operator[](size_t index) const;
        
        /** @brief Returns the height of the Kernel. **/
        int getHeight() const;
        
        /** @brief Returns the width of the Kernel. **/
        int getWidth() const;
};

Kernel::Kernel(const std::vector<std::vector<float>>& kernelData) {
    if (kernelData.size() == 0)
        throw std::invalid_argument("std::invalid_argument: Kernel::Kernel() -> matrix can't be empty.");

    const unsigned int width = kernelData[0].size();
    for (unsigned int rowIndex = 1; rowIndex < kernelData.size(); rowIndex++)
        if (kernelData[rowIndex].size() != width)
            throw std::invalid_argument("std::invalid_argument: Kernel::Kernel() -> matrix must not have constant dimensions.");

    if (kernelData.size() % 2 || width % 2)
        throw std::invalid_argument("std::invalid_argument: Kernel::Kernel() -> matrix must have odd dimensions.");

    this->kernelData = kernelData;
}

Kernel::Kernel(const Kernel& kernel) {
    for (int row = 0; row < kernel.getHeight(); row++)
        kernelData.push_back(kernel[row]);
}

void Kernel::normalize() {
    float sum = 0;
    for (const auto& row : kernelData)
        for (const auto& column : row)
            sum += column;

    for (auto& row : kernelData)
        for (auto& column : row)
            column *= 1 / sum;
}

std::vector<float> Kernel::operator[](size_t index) const {
    return kernelData.at(index);
}

int Kernel::getHeight() const {
    return kernelData.size();
}

int Kernel::getWidth() const {
    return kernelData[0].size();
}

Kernel getGaussianKernel(int width, int height, float variance) {
    if (width % 2 || height % 2)
        throw std::invalid_argument("std::invalid_argument: getGaussianKernel() -> kernel must have odd dimensions.");

    std::vector<std::vector<float>> kernelData;

    const float center_x = width / 2;
    const float center_y = height / 2;

    for (int height_index = 0; height_index < height; height_index++) {
        std::vector<float> kernelRow;

        for (int width_index = 0; width_index < width; width_index++) {
            const float probability = gaussianProbabilty(abs(width_index - center_x), 0, variance) *
                                      gaussianProbabilty(abs(height_index - center_y), 0, variance);

            kernelRow.push_back(probability);
        }

        kernelData.push_back(kernelRow);
    }

    return Kernel(kernelData);
}
