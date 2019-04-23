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

            for (int k_row = -kernel.getHeight() / 2; k_row <= kernel.getHeight() / 2; k_row++)
                for (int k_col = -kernel.getWidth() / 2; k_col <= kernel.getWidth() / 2; k_col++) {
                    int used_row = row + k_row;
                    int used_col = col + k_col;

                    if (used_col < 0 || used_row < 0) { 
                        used_row = pixelData.getHeight() - 1 - k_row + 1;
                        used_col = pixelData.getWidth() - 1 - k_col + 1;
                    }

                    if (used_col >= (int)pixelData.getWidth() || used_row >= (int)pixelData.getHeight()) {
                        used_row = k_row - 1;
                        used_col = k_col - 1;
                    }

                    result += pixelData.getColor(used_col, used_row) *
                              kernel[k_row + kernel.getHeight() / 2][k_col + kernel.getWidth() / 2];
                }

            output.setColor(col, row, result);
        }

    ofImage outputImg;
    outputImg.setFromPixels(output.getData(), output.getWidth(), output.getHeight(), OF_IMAGE_COLOR);
                
    return outputImg;
} 


void ofApp::setup(){
    source.load("img.bmp");

    GaussianKernel kernel(1, 5, 5);
    kernel.normalize();

    result = convolve(source, kernel);

    for (int i = 0; i < 3; i++)
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
