#include "ofApp.h"

#include "kernel.h"
#include "image_convolve.h"

void ofApp::setup(){
    source.load("img.bmp");

    try {
        GaussianKernel kernel(5, 5, 1);

        kernel.normalize();

        result = convolve(source, kernel);

        result = convolve(result, kernel);
        result = convolve(result, kernel);
        result = convolve(result, kernel);
        result = convolve(result, kernel);
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
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
