#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

#include "gauss_distribution.h"

bool aproximately_equal(const float nr_1, const float nr_2) {
    return std::abs(nr_1 - nr_2) < 0.00001;
}

class ofApp : public ofxUnitTestsApp {
    void run_gaussian_test() {
        ofLogNotice() << "Testing \'gaussianProbability\'";

        ofxTest(aproximately_equal(gaussianProbabilty(1, 0, 1), 0.24197072), 
                "x=1, mean=0, variance=1");
        ofxTest(aproximately_equal(gaussianProbabilty(-1, 0, 1), 0.24197072), 
                "x=-1, mean=0, variance=1");
        ofxTest(aproximately_equal(gaussianProbabilty(1000, 900, 2000), 0.0007322), 
                "x=1000, mean=900, variance=2000");
        ofxTest(aproximately_equal(gaussianProbabilty(2, 0, 0.5), 0.0103335),
                "x=2, mean=0, variance=0.5");
        ofxTest(aproximately_equal(gaussianProbabilty(2, 0, 2), 0.10378),
                "x=2, mean=0, variance=2");
        ofxTest(aproximately_equal(gaussianProbabilty(-30, -50, 40), 0.0004250),
                "x=-30, mean=-50, variance=40");
    }

    void run() {
        run_gaussian_test();
    }
};

int main() {
    ofInit();

    auto window = std::make_shared<ofAppNoWindow>();
    auto app = std::make_shared<ofApp>();

    ofRunApp(window, app);

    return ofRunMainLoop();
}
