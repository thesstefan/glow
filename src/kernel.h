#pragma once

#include <vector>

static constexpr float E = 2.71828183;

class Kernel {
    private:
        std::vector<std::vector<float>> kernelData;

    public:
        Kernel(const std::vector<std::vector<float>>& kernelData);
        Kernel(const Kernel& kernelData);

        // Creates a Gaussian kernel using the sigma of the distribution.
        Kernel(const float sigma, int width, int height);

        void normalize();

        std::vector<float> operator[](size_t index) const;
        Kernel operator*(float multiplier);

        int getHeight() const;
        int getWidth() const;
};
