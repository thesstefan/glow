#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

#include "image_convolve.h"

// Checks if ucharImage, the converted version of floatImage is looking as expected.
bool isConvertedRight(const ofImage &ucharImage, const ofFloatImage &floatImage) {
    if (ucharImage.getHeight() != floatImage.getHeight() ||
        ucharImage.getWidth() != floatImage.getWidth())
            return false;

    for (size_t index = 0; index < ucharImage.getWidth() * ucharImage.getHeight(); index++) {
        ofColor_<float> floatColor = floatImage.getColor(index);

        ofColor expectedColor = ofColor(static_cast<unsigned char>(floatColor.r),       
                                        static_cast<unsigned char>(floatColor.g),
                                        static_cast<unsigned char>(floatColor.b));

        if (expectedColor != ucharImage.getColor(index))
            return false;
    }

    return true;
}

/** 
 * Returns an width x height image, with a white pixel at pixel_x, pixel_y
 * (the rest of the pixels are black)
 */
template <typename PixelType>
ofImage_<PixelType> getOnePixelImage(const size_t width, const size_t height,
                                     const size_t pixel_x, const size_t pixel_y) {
    ofPixels_<PixelType> output;
    output.allocate(width, height, OF_PIXELS_RGB);

    output.setColor(ofColor(0, 0, 0));
    output.setColor(pixel_x, pixel_y, ofColor(255, 255, 255));

    ofImage_<PixelType> outputImg;
    outputImg.setFromPixels(output.getData(), 
                            output.getWidth(), 
                            output.getHeight(), 
                            OF_IMAGE_COLOR);

    return outputImg;
}

// Checks if an image with one white pixel and the others black is convolved correctly.
template <typename PixelType>
bool isOnePixelImageConvolved(const ofImage_<PixelType> &image, const Kernel &kernel, 
                                  const size_t pixel_x, const size_t pixel_y) {
    size_t usedWidth = 0;
    size_t usedHeight = 0;

    for (int width = (int)(pixel_x - kernel.getWidth() / 2); 
             width <= (int)(pixel_x + kernel.getWidth() / 2); 
             width++)
        for (int height = (int)(pixel_y - kernel.getHeight() / 2);
                 height <= (int)(pixel_y + kernel.getHeight() / 2);
                 height++) {

            if (width < 0)
                usedWidth = image.getWidth() + width;
            else if (width >= (int)image.getWidth())
                usedWidth = width - pixel_x - 1;
            else
                usedWidth = width;

            if (height < 0)
                usedHeight = image.getHeight() + height;
            else if (height >= (int)image.getWidth())
                usedHeight = height - pixel_y - 1;
            else 
                usedHeight = height;

            const ofColor_<PixelType> color = image.getColor(usedWidth, usedHeight);

            const float colorValue = 255 * kernel[height - pixel_y + kernel.getHeight() / 2]
                                                  [width - pixel_x + kernel.getWidth() / 2];

            const ofColor_<PixelType> expectedColor(colorValue, colorValue, colorValue);

            if (color != expectedColor)
                return false;
        }

    const ofColor black = ofColor(0, 0, 0);
    size_t colorPixels = 0;

    for (size_t width = 0; width < image.getWidth(); width++)
        for (size_t height = 0; height < image.getHeight(); height++)
            if (image.getColor(width, height) != black)
                colorPixels++;

    return colorPixels == kernel.getWidth() * kernel.getHeight();
}

template <typename PixelType>
bool areImagesEqual(const ofImage_<PixelType> &image, const ofImage_<PixelType> &image_) {
    if (image.getHeight() != image_.getHeight() ||
        image.getWidth() != image_.getWidth())

    for (size_t width = 0; width < image.getWidth(); width++)
        for (size_t height = 0; height < image.getHeight(); height++)
            if (image.getColor(width, height) != image_.getColor(width, height))
                return false;

    return true;
}

// Checks if the average brightness of two images is roughly the same.
template <typename PixelType>
bool hasSameBrightness(const ofImage &image, const ofImage_<PixelType> &image_, 
                               const float maxError) {
    
    if (image.getHeight() != image_.getHeight() ||
        image.getWidth() != image_.getWidth())
            return false;

    unsigned long brightness = 0;
    unsigned long brightness_ = 0;

    for (size_t width = 0; width < image.getWidth(); width++)
        for (size_t height = 0; height < image.getHeight(); height++) {
            brightness += image.getColor(width, height).getBrightness();
            brightness_ += image_.getColor(width, height).getBrightness();
        }

    size_t imageSize = image.getWidth() * image.getHeight();

    return std::abs(float((brightness - brightness_) / imageSize)) < maxError;
}

class ofApp : public ofxUnitTestsApp {
    void convert_test() {
        ofLogNotice() << "Testing convertToImage()";

        ofFloatImage input;
        input.load("test_img.bmp");

        ofxTest(isConvertedRight(convertToImage(input), input),
                "decimal part = 0");

        // Check if the conversions from a float with a decimal part != 0 to unsigned
        // char is handled correctly.
        for (size_t index = 0; index < input.getWidth() * input.getHeight(); index++)
            input.setColor(index, input.getColor(index) + 0.5);

        ofxTest(isConvertedRight(convertToImage(input), input),
                "decimal part = 0.5");
    }

    void convolve_test() {
        ofLogNotice() << "Testing convolve()";

        const Kernel kernel(std::vector<std::vector<double>> {
            {0.0625, 0.125, 0.0625},
            {0.125,  0.25,  0.125},
            {0.0625, 0.125, 0.0625},
        });

        ofxTest(isOnePixelImageConvolved<unsigned char>(
                    convolve(getOnePixelImage<unsigned char>(101, 101, 50, 50), kernel),
                    kernel, 50, 50), 
                "center");

        ofxTest(isOnePixelImageConvolved<unsigned char>(
                    convolve(getOnePixelImage<unsigned char>(101, 101, 50, 0), kernel),
                    kernel, 50, 0), 
                "up");

        ofxTest(isOnePixelImageConvolved<unsigned char>(
                    convolve(getOnePixelImage<unsigned char>(101, 101, 50, 100), kernel),
                    kernel, 50, 100), 
                "down");

        ofxTest(isOnePixelImageConvolved<unsigned char>(
                    convolve(getOnePixelImage<unsigned char>(101, 101, 0, 50), kernel),
                    kernel, 0, 50), 
                "left");

        ofxTest(isOnePixelImageConvolved<unsigned char>(
                    convolve(getOnePixelImage<unsigned char>(101, 101, 100, 50), kernel),
                    kernel, 100, 50), 
                "right");

        ofImage image;
        image.load("test_img.bmp");

        convolve(image, kernel).save("ala.bmp");

        ofxTest(hasSameBrightness<unsigned char>
                    (image, convolve(image, kernel), 0.00001),
                "brightness -> maxError = 0.00001");
    }

    void float_convolve_test() {
        ofLogNotice() << "Testing float_convolve()";

        const Kernel kernel(std::vector<std::vector<double>> {
            {0.0625, 0.125, 0.0625},
            {0.125,  0.25,  0.125},
            {0.0625, 0.125, 0.0625},
        });

        ofxTest(isOnePixelImageConvolved<float>(
                    float_convolve(getOnePixelImage<float>(101, 101, 50, 50), kernel),
                    kernel, 50, 50), 
                "center");

        ofxTest(isOnePixelImageConvolved<float>(
                    float_convolve(getOnePixelImage<float>(101, 101, 50, 0), kernel),
                    kernel, 50, 0), 
                "up");

        ofxTest(isOnePixelImageConvolved<float>(
                    float_convolve(getOnePixelImage<float>(101, 101, 50, 100), kernel),
                    kernel, 50, 100), 
                "down");

        ofxTest(isOnePixelImageConvolved<float>(
                    float_convolve(getOnePixelImage<float>(101, 101, 0, 50), kernel),
                    kernel, 0, 50), 
                "left");

        ofxTest(isOnePixelImageConvolved<float>(
                    float_convolve(getOnePixelImage<float>(101, 101, 100, 50), kernel),
                    kernel, 100, 50), 
                "right");

        ofImage image;
        image.load("test_img.bmp");

        ofxTest(hasSameBrightness<float>
                    (image, float_convolve(image, kernel), 0.00001),
                "brightness -> maxError = 0.00001");
    }

    void iterative_convolve_test() {
        ofLogNotice() << "Testing iterativeConvolve()";

        const Kernel kernel(std::vector<std::vector<double>> {
            {0.0625, 0.125, 0.0625},
            {0.125,  0.25,  0.125},
            {0.0625, 0.125, 0.0625},
        });

        ofImage image;
        image.load("test_img.bmp");

        ofImage result = iterativeConvolve(image, kernel, 5);

        ofFloatImage floatImage;
        image.load("test_img.bmp");

        for (int times = 0; times < 5; times++)
            floatImage = float_convolve(floatImage, kernel);

        ofxTest(areImagesEqual(convertToImage(floatImage), result),
                "5 times");
    }

    void run() {
        ofLogNotice() << "TESTING convolve MODULE\n";

        convert_test();
        ofLogNotice() << '\n';

        convolve_test();
        ofLogNotice() << '\n';

        float_convolve_test();
        ofLogNotice() << '\n';
        
        iterative_convolve_test();
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
