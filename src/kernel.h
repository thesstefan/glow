#pragma once

#include <vector>

static constexpr float E = 2.71828183;

class Kernel {
    private:
        std::vector<std::vector<float>> kernelData;

    public:
        Kernel(const std::vector<std::vector<float>>& kernelData);
        Kernel(const Kernel& kernelData);

        void normalize();

        std::vector<float> operator[](size_t index) const;
        Kernel operator*(float multiplier);

        int getHeight() const;
        int getWidth() const;
};

class GaussianKernel : public Kernel {
    public:
        GaussianKernel(float sigma, int width, int height);
};
