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
         * @warning The input matrix must not be empty, have even dimensions, or
         *          different row-dimensions. Otherwise, std::invalid_argument is thrown.
         */
        Kernel(const std::vector<std::vector<float>>& kernelData);

        /** @brief Copy-construct the Kernel. **/
        Kernel(const Kernel& kernel);

        /** @brief Normalizes the Kernel values. **/
        void normalize();

        /** @brief Returns the specified row of the Kernel. **/
        std::vector<float> operator[](const size_t index) const;
        
        /** @brief Returns the height of the Kernel. **/
        int getHeight() const;
        
        /** @brief Returns the width of the Kernel. **/
        int getWidth() const;
};

class GaussianKernel : public Kernel {
    private:
        /**
         * @brief Extracts the necessary data from a normal distribution with 
         *        mean 0 and a given variance to create a Kernel.
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
        std::vector<std::vector<float>> getGaussianData(const int width, 
                                                        const int height, 
                                                        const float variance) const;

    public:
        /**
         * @brief Creates a kernel from a normal distribution 
         *        with mean 0 and a given variance.
         *
         * Calls getGaussianData(). All requirements apply.
         */
        GaussianKernel(const int width, const int height, const float variance);
};
