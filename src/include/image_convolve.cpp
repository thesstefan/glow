#include "image_convolve.h"

#include <iostream>

template <typename InputType, typename OutputType>
ofImage_<OutputType> convert(const ofImage_<InputType> &input) {
    const ofPixels_<InputType> pixels = input.getPixels();

    ofPixels_<OutputType> output;
    output.allocate(pixels.getWidth(), pixels.getHeight(),
                    OF_PIXELS_RGB);

    for (size_t row = 0; row < pixels.getHeight(); row++)
        for (size_t col = 0; col < pixels.getWidth(); col++) {
            ofColor_<InputType> color = input.getColor(col, row);

            output.setColor(col, row, 
                            ofColor(static_cast<OutputType>(color.r), 
                                    static_cast<OutputType>(color.g), 
                                    static_cast<OutputType>(color.b)));
        }

    ofImage_<OutputType> outputImg;
    outputImg.setFromPixels(output.getData(), 
                            output.getWidth(), 
                            output.getHeight(), 
                            OF_IMAGE_COLOR);
                
    return outputImg;
}

template <typename PixelType>
ofImage convertToImage(const ofImage_<PixelType> &input) {
    return convert<PixelType, unsigned char>(input);
}

template <typename InputType, typename OutputType>
ofImage_<OutputType> convolve_(const ofImage_<InputType> &input, 
                               const Kernel &kernel) {
    const ofPixels_<InputType> pixels = input.getPixels();

    ofPixels_<OutputType> output;
    output.allocate(pixels.getWidth(), pixels.getHeight(),
                    OF_PIXELS_RGB);

    // Avoid size_t to int conversions.
    int kernelHeight = kernel.getHeight();
    int kernelWidth = kernel.getWidth();

    for (size_t row = 0; row < pixels.getHeight(); row++)
        for (size_t col = 0; col < pixels.getWidth(); col++) {
            float red = 0;
            float green = 0;
            float blue = 0;

            for (int kernelRow = -kernelHeight / 2; 
                     kernelRow <= kernelHeight / 2; 
                     kernelRow++)

                for (int kernelCol = -kernelWidth / 2;
                         kernelCol <= kernelWidth / 2;
                         kernelCol++) {

                    int usedRow = row + kernelRow;
                    int usedCol = col + kernelCol;

                    if (usedRow < 0)
                        usedRow = pixels.getHeight() + kernelRow;
                    else if (usedRow >= (int)pixels.getHeight())
                        usedRow = kernelRow - 1;

                    if (usedCol < 0) 
                        usedCol = pixels.getWidth() + kernelCol;
                    else if (usedCol >= (int)pixels.getWidth())
                        usedCol = kernelCol - 1;

                    const double multiplier = kernel[kernelRow + kernelHeight / 2]
                                                    [kernelCol + kernelWidth / 2];

                    const ofColor_<InputType> color = pixels.getColor(usedCol, usedRow);

                    red   += static_cast<OutputType>(color.r) * multiplier;
                    green += static_cast<OutputType>(color.g) * multiplier;
                    blue  += static_cast<OutputType>(color.b) * multiplier;
                }

            output.setColor(col, row, ofColor_<OutputType>(red, green, blue, 255));
        }
    
    ofImage_<OutputType> outputImg;
    outputImg.setFromPixels(output.getData(), 
                            output.getWidth(), 
                            output.getHeight(), 
                            OF_IMAGE_COLOR);

    return outputImg;
}

template <typename PixelType>
ofFloatImage float_convolve(const ofImage_<PixelType> &input, const Kernel &kernel) {
    return convolve_<PixelType, float>(input, kernel);
}

template <typename PixelType>
ofImage convolve(const ofImage_<PixelType> &input, const Kernel &kernel) {
    return convolve_<PixelType, unsigned char>(input, kernel);
}

template <typename PixelType>
ofImage iterativeConvolve(const ofImage_<PixelType> &image, const Kernel &kernel,
                          const int times) {
    ofFloatImage result = float_convolve<PixelType>(image, kernel);

    for (int index = 1; index < times; index++)
        result = float_convolve<float>(result, kernel);

    return convertToImage<float>(result);
}

template ofImage convolve<unsigned char>(const ofImage &input, const Kernel &kernel);

template ofFloatImage float_convolve<unsigned char>(const ofImage &input, const Kernel &kernel);
template ofFloatImage float_convolve<float>(const ofFloatImage &input, const Kernel &kernel);

template ofImage convertToImage<float>(const ofFloatImage &input);

template ofImage iterativeConvolve<unsigned char>(const ofImage& input, const Kernel& kernel, const int times);
