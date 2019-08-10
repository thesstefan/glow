#pragma once

#include <stdexcept>

#include "ofImage.h"

#include "kernel.h"
#include "image_convolve.h"

ofImage blend(const ofImage &top, const ofImage &bottom) {
    ofPixels output;
    output.allocate(top.getWidth(), top.getHeight(),
                    OF_PIXELS_RGB);

    for (size_t row = 0; row < top.getHeight(); row++)
        for (size_t col = 0; col < top.getWidth(); col++) {
            ofColor topColor = top.getColor(col, row);

            if (topColor != ofColor(0, 0, 0))
                output.setColor(col, row, topColor);
            else
                output.setColor(col, row, bottom.getColor(col, row));
        }

    ofImage outputImg;
    outputImg.setFromPixels(output.getData(), 
                            output.getWidth(), 
                            output.getHeight(), 
                            OF_IMAGE_COLOR);
                
    return outputImg;
}

/**
 * @brief Applies a glow effect on an image by drawing
 *        the blurred image over the given one.
 *
 * @param image -> The image to be glowing.
 *
 * @param kType -> The type of the GaussianKernel.
 * @param kWidth -> The width of the GaussianKernel.
 * @param kHeight -> The height of the GaussianKernel.
 * @param kVariance -> The variance of the GaussianKernel.
 */
template <typename kType, size_t kWidth, size_t kHeight>
ofImage glow(const ofImage &image,
             const double kVariance) {
    GaussianKernel<kType, 1, kHeight> vertical(kVariance);
    GaussianKernel<kType, kWidth, 1> horizontal(kVariance);

    vertical.normalize();
    horizontal.normalize();

    ofImage blurred;

    blurred = convolve<kType, 1, kHeight>(image, vertical);
    blurred = convolve<kType, kWidth, 1>(blurred, horizontal);

    return blend(image, blurred);
}
