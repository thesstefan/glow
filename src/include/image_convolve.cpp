#include "image_convolve.h"

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

// Handles convolution at the margins of the image. (mirroring)
size_t adjustPosition(const int coordinate, 
                      const int kernelCoordinate,
                      const int max) {
    const int unadjustedPosition = coordinate + kernelCoordinate;

    if (unadjustedPosition < max && unadjustedPosition >= 0)
        return unadjustedPosition;

    if (unadjustedPosition < 0)
        return max + kernelCoordinate;

    return unadjustedPosition % max;
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

                    const size_t usedRow = adjustPosition(row, kernelRow, pixels.getHeight());
                    const size_t usedCol = adjustPosition(col, kernelCol, pixels.getWidth());

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

template <typename InputType, typename OutputType>
ofImage_<OutputType> iterativeConvolve_(const ofImage_<InputType> &image, 
                                        const Kernel &kernel,
                                        const int times) {
    ofFloatImage result = convolve_<InputType, float>(image, kernel);

    for (int index = 1; index < times; index++)
        result = convolve_<float, float>(result, kernel);

    return convert<float, OutputType>(result);
}

ofImage convertToImage(const ofFloatImage &input) {
    return convert<float, unsigned char>(input);
}

ofFloatImage float_convolve(const ofImage &input, const Kernel &kernel) {
    return convolve_<unsigned char, float>(input, kernel);
}

ofImage convolve(const ofImage &input, const Kernel &kernel) {
    return convolve_<unsigned char, unsigned char>(input, kernel);
}

ofImage iterativeConvolve(const ofImage &image, 
                          const Kernel &kernel,
                          const int times) {
    return iterativeConvolve_<unsigned char, unsigned char>(image, kernel, times);
}
