#include "glow.h"

#include "image_convolve.h"

#include <stdexcept>

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

ofImage glow(const ofImage &image, const Kernel &kernel) {
    ofImage blurred;

    try {
        blurred = convolve(image, kernel);
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }

    return blend(image, blurred);
}
