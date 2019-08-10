#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

#include "kernel.h"

template <typename Type, size_t width, size_t height>
bool isKernelEqual(const Kernel<Type, width, height> &kernel_1, 
                   const Kernel<Type, width, height> &kernel_2) {
    for (size_t row = 0; row < height; row++)
        for (size_t col = 0; col < width; col++)
            if (std::abs(kernel_1[row][col] - kernel_2[row][col]) > 0.0001)
                return false;

    return true;
}

class ofApp : public ofxUnitTestsApp {
    void gaussian_kernel_test() {
        ofLogNotice() << "Testing GaussianKernel::GaussianKernel()";

        GaussianKernel<double, 7, 7> gauss_1(0.707106789);

        Kernel<double, 7, 7> kernel_1 (
        {{
            {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
            {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
            {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
            {0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771},
            {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
            {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
            {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067}
        }});

        ofxTest(isKernelEqual<double, 7, 7>(kernel_1, gauss_1), 
                "size:7x7, variance=0.707106789");
    }

    void kernel_normalize_test() {
        ofLogNotice() << "Testing Kernel::normalize()";

        Kernel<double, 3, 3> kernel_1 (
        {{
                {1, 1, 1},
                {1, 1, 1},
                {1, 1, 1}
        }});

        kernel_1.normalize();

        Kernel<double, 3, 3> normalized_1(
        {{ 
                {1./9, 1./9, 1./9},
                {1./9, 1./9, 1./9},
                {1./9, 1./9, 1./9}
        }});

        ofxTest(isKernelEqual<double, 3, 3>(kernel_1, normalized_1),
                "kernel_1");

        Kernel<double, 3, 3> kernel_2(
        {{
                {1, 2, 1},
                {2, 4, 2},
                {1, 2, 1}
        }});

        kernel_2.normalize();

        Kernel<double, 3, 3> normalized_2(
        {{ 
                {1./16, 2./16, 1./16},
                {2./16, 4./16, 2./16},
                {1./16, 2./16, 1./16}
        }});

        ofxTest(isKernelEqual<double, 3, 3>(kernel_2, normalized_2),
                "kernel_2");

        Kernel<double, 5, 5> kernel_3(
        {{
                {1, 2, 4,  2, 1},
                {2, 4, 8,  4, 2},
                {4, 8, 16, 8, 4},
                {2, 4, 8,  4, 2},
                {1, 2, 4,  2, 1}
        }});

        kernel_3.normalize();

        Kernel<double, 5, 5> normalized_3(
        {{
                {1./100, 2./100, 4./100,  2./100, 1./100},
                {2./100, 4./100, 8./100,  4./100, 2./100},
                {4./100, 8./100, 16./100, 8./100, 4./100},
                {2./100, 4./100, 8./100,  4./100, 2./100},
                {1./100, 2./100, 4./100,  2./100, 1./100}
        }});

        ofxTest(isKernelEqual<double, 5, 5>(kernel_3, normalized_3),
                "kernel_3");
    }

        void run() {
            ofLogNotice() << '\n';
            ofLogNotice() << "TESTING kernel MODULE\n";

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
