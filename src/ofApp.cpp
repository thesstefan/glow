#include "ofApp.h"

#include "kernel.h"
#include "image_convolve.h"
#include "glow.h"

void ofApp::setup(){
    try {
        source.load("input.bmp");

        GaussianKernel<double, 25, 25> kernel(10);

        kernel.normalize();

        result = glow<double, 25, 25>(source, kernel);

        result.save("output.bmp");
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
}

void ofApp::draw() {
    result.draw(0, 0);
}

