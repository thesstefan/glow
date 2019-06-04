#pragma once

#include "ofImage.h"
#include "kernel.h"

ofImage convolve(const ofImage &input, const Kernel &kernel) {
    const ofPixels pixels = input.getPixels();

    ofPixels output;
    output.allocate(pixels.getWidth(), pixels.getHeight(),
                    OF_IMAGE_COLOR);

    for (size_t row = 0; row < pixels.getHeight(); row++)
        for (size_t col = 0; col < pixels.getWidth(); col++) {
            ofColor result(0, 0, 0);

            for (int kernelRow = -(kernel.getHeight() / 2); 
                     kernelRow <= (int)kernel.getHeight() / 2; 
                     kernelRow++)

                for (int kernelCol = -(kernel.getWidth() / 2); 
                         kernelCol <= (int)kernel.getWidth() / 2; 
                         kernelCol++) {

                    int usedRow = row + kernelRow;
                    int usedCol = col + kernelCol;

                    if (usedCol < 0) 
                        usedCol = pixels.getWidth() + kernelCol;
                    else if (usedCol > (int)kernel.getWidth())
                        usedCol = kernelCol - 1;

                    if (usedRow < 0)
                        usedRow = pixels.getHeight() + kernelRow;
                    else if (usedRow > (int)kernel.getHeight())
                        usedRow = kernelRow - 1;

                    result += pixels.getColor(usedCol, usedRow) *
                              kernel[kernelRow + kernel.getHeight() / 2]
                                    [kernelCol + kernel.getWidth() / 2];
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
