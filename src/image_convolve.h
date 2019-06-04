#pragma once

#include "ofImage.h"
#include "kernel.h"

/**
 * @brief Convolves an image with a Kernel. 
 *
 * Used to blur an image by using a gaussian Kernel.
 *
 * @param input -> The image to be used.
 * @param kernel -> The kernel to be used.
 */
ofImage convolve(const ofImage &input, const Kernel &kernel) {
    const ofPixels pixels = input.getPixels();

    ofPixels output;
    output.allocate(pixels.getWidth(), pixels.getHeight(),
                    OF_IMAGE_COLOR);

    // Avoid size_t to int conversions.
    int kernelHeight = kernel.getHeight();
    int kernelWidth = kernel.getWidth();

    for (size_t row = 0; row < pixels.getHeight(); row++)
        for (size_t col = 0; col < pixels.getWidth(); col++) {
            ofColor result(0, 0, 0);

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

                    result += pixels.getColor(usedCol, usedRow) *
                              kernel[kernelRow + kernelHeight / 2]
                                    [kernelCol + kernelWidth / 2];
                }

            output.setColor(col, row, result);
        }

    ofImage outputImg;
    outputImg.setFromPixels(output.getData(), 
                            output.getWidth(), 
                            output.getHeight(), 
                            OF_IMAGE_COLOR);
                
    return outputImg;
}
