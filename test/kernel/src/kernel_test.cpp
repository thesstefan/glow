#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

#include "kernel.h"

bool isKernelEqual(const Kernel &kernel_1, const Kernel &kernel_2) {
    if (kernel_1.getHeight() != kernel_2.getHeight() || kernel_1.getWidth() != kernel_2.getWidth())
        return false;

    for (size_t row = 0; row < kernel_1.getHeight(); row++)
        for (size_t col = 0; col < kernel_1.getWidth(); col++)
            if (std::abs(kernel_1[row][col] - kernel_2[row][col]) > 0.0001)
                return false;

    return true;
}

class ofApp : public ofxUnitTestsApp {
    void gaussian_kernel_test() {
        ofLogNotice() << "Testing GaussianKernel::GaussianKernel()";

        GaussianKernel gauss_1(7, 7, 0.707106789);

        Kernel kernel_1(std::vector<std::vector<double>> {
                {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
                {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
                {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
                {0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771},
                {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
                {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
                {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067}
        });

        ofxTest(isKernelEqual(kernel_1, gauss_1), 
                "size:7x7, variance=0.707106789");
    }

    void kernel_normalize_test() {
        ofLogNotice() << "Testing Kernel::normalize()";

        Kernel kernel_1(std::vector<std::vector<double>> {
                {1, 1, 1},
                {1, 1, 1},
                {1, 1, 1}
        });

        kernel_1.normalize();

        Kernel normalized_1(std::vector<std::vector<double>> { 
                {1./9, 1./9, 1./9},
                {1./9, 1./9, 1./9},
                {1./9, 1./9, 1./9}
        });

        ofxTest(isKernelEqual(kernel_1, normalized_1),
                "kernel_1");

        Kernel kernel_2(std::vector<std::vector<double>> {
                {1, 2, 1},
                {2, 4, 2},
                {1, 2, 1}
        });

        kernel_2.normalize();

        Kernel normalized_2(std::vector<std::vector<double>> { 
                {1./16, 2./16, 1./16},
                {2./16, 4./16, 2./16},
                {1./16, 2./16, 1./16}
        });

        ofxTest(isKernelEqual(kernel_2, normalized_2),
                "kernel_2");

        Kernel kernel_3(std::vector<std::vector<double>> {
                {1, 2, 4,  2, 1},
                {2, 4, 8,  4, 2},
                {4, 8, 16, 8, 4},
                {2, 4, 8,  4, 2},
                {1, 2, 4,  2, 1}
        });

        kernel_3.normalize();

        Kernel normalized_3(std::vector<std::vector<double>> {
                {1./100, 2./100, 4./100,  2./100, 1./100},
                {2./100, 4./100, 8./100,  4./100, 2./100},
                {4./100, 8./100, 16./100, 8./100, 4./100},
                {2./100, 4./100, 8./100,  4./100, 2./100},
                {1./100, 2./100, 4./100,  2./100, 1./100}
        });

        ofxTest(isKernelEqual(kernel_3, normalized_3),
                "kernel_3");
    }

        void run() {
            ofLogNotice() << "\nTESTING kernel MODULE\n";

            gaussian_kernel_test();
            ofLogNotice() << '\n';

            kernel_normalize_test();
            ofLogNotice() << '\n';
        }
};

int main() {
    ofInit();

    auto window = std::make_shared<ofAppNoWindow>();
    auto app = std::make_shared<ofApp>();

    ofRunApp(window, app);

    return ofRunMainLoop();
}
