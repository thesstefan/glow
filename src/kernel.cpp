#include "kernel.h"

#include <cmath>
#include "ofMathConstants.h"

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

Kernel::Kernel(const float sigma, int width, int height) {
    const float multiplier = 1. / (2 * PI * sigma * sigma);

    for (int row = -height / 2; row <= height / 2; row++) {
        std::vector<float> kernelRow;

        for (int column = -width / 2; column <= width / 2; column++) {
            const float power = -(float)(column * column + row * row) / (2 * sigma * sigma);

            kernelRow.push_back(multiplier * std::pow(E, power));
        }

        kernelData.push_back(kernelRow);
    }
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
