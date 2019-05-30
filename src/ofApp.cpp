#include "ofApp.h"

#include "kernel.h"

ofImage convolve(const ofImage& source, 
                 const Kernel& kernel) {
    auto pixelData = source.getPixels();

    ofPixels output;
    output.allocate(pixelData.getWidth(), 
                    pixelData.getHeight(),
                    OF_IMAGE_COLOR);

    for (size_t row = 0; row < pixelData.getHeight(); row++)
        for (size_t col = 0; col < pixelData.getWidth(); col++) {
            ofColor result(0, 0, 0);

            for (int kernel_row = -kernel.getHeight() / 2; kernel_row <= kernel.getHeight() / 2; kernel_row++)
                for (int kernel_col = -kernel.getWidth() / 2; kernel_col <= kernel.getWidth() / 2; kernel_col++) {
                    int used_row = row + kernel_row;
                    int used_col = col + kernel_col;

                    if (used_col < 0 || used_row < 0) { 
                        used_row = pixelData.getHeight() - 1 - kernel_row + 1;
                        used_col = pixelData.getWidth() - 1 - kernel_col + 1;
                    }

                    if (used_col >= (int)pixelData.getWidth() || used_row >= (int)pixelData.getHeight()) {
                        used_row = kernel_row - 1;
                        used_col = kernel_col - 1;
                    }

                    result += pixelData.getColor(used_col, used_row) *
                              kernel[kernel_row + kernel.getHeight() / 2][kernel_col + kernel.getWidth() / 2];
                }

            output.setColor(col, row, result);
        }

    ofImage outputImg;
    outputImg.setFromPixels(output.getData(), output.getWidth(), output.getHeight(), OF_IMAGE_COLOR);
                
    return outputImg;
} 


void ofApp::setup(){
    source.load("img.bmp");

    Kernel kernel = getGaussianKernel(5, 5, 1);
    kernel.normalize();

    result = convolve(source, kernel);
    result = convolve(result, kernel);
    result = convolve(result, kernel);
    result = convolve(result, kernel);
}

void ofApp::draw() {
    ofFbo fbo;

    fbo.begin();
    result.draw(0, 0);
    // source.draw(0, 0);

    source.draw(0, source.getHeight() + 20);
    fbo.end();

    fbo.draw(0, 0);
}
