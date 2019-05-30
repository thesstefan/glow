#pragma once

#include <vector>

#include "gauss_distribution.h"

class Kernel {
    private:
        std::vector<std::vector<float>> kernelData;

    public:
        Kernel(const std::vector<std::vector<float>>& kernelData);
        Kernel(const Kernel& kernel);

        void normalize();

        std::vector<float> operator[](size_t index) const;
        Kernel operator*(float multiplier);

        int getHeight() const;
        int getWidth() const;
};

Kernel::Kernel(const std::vector<std::vector<float>>& kernelData) :
    kernelData(kernelData) {}

Kernel::Kernel(const Kernel& kernel) {
    for (int row = 0; row < kernel.getHeight(); row++)
        kernelData.push_back(kernel[row]);
}

void Kernel::normalize() {
    float sum = 0;

    for (const auto& row : kernelData)
        for (const auto& column : row)
            sum += column;

    *this = *this * ((float)1 / sum);
}

std::vector<float> Kernel::operator[](size_t index) const {
    return kernelData.at(index);
}

Kernel Kernel::operator*(float multiplier) {
    for (auto &row : kernelData)
        for (auto &column : row) 
            column *= multiplier;

    return *this;
}

int Kernel::getHeight() const {
    return kernelData.size();
}

int Kernel::getWidth() const {
    return kernelData[0].size();
}

Kernel getGaussianKernel(int width, int height, float variance) {
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
