#include "ofApp.h"

#include "kernel.h"
#include "image_convolve.h"
#include "glow.h"

void ofApp::setup(){
    try {
        source.load("input.bmp");

        result = glow<double, 25, 25>(source, 10);

        result.save("output.bmp");
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
}

void ofApp::draw() {
    result.draw(0, 0);
}

