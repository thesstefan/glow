#pragma once

#include <vector>

/** @brief Small matrix used for image processing. **/
class Kernel {
    private:
        /** @brief The values of the Kernel. **/
        std::vector<std::vector<float>> kernelData;

    public:
        /** 
         * @brief Constructs the Kernel. 
         *
         * @warning The input matrix must not be empty or have even or variable
         *          dimensions. Otherwise, std::invalid_argument is thrown.
         */
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

class GaussianKernel : public Kernel {
    public:
        /**
        * @brief Creates a gaussian kernel from a normal distribution 
        *        with mean 0 and a given variance.
        *
        * @param width -> The width of the Kernel.
        * @param height -> The height of the Kernel.
        *
        * @param variance -> The variance of the distribution used to create
        *                    the Kernel.
        *
        * @warning width and height must be positive and odd. std::invalid_argument
        *          is thrown otherwise.
        */
        GaussianKernel(int width, int height, float variance);
};
