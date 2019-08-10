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

size_t marginsHandling(const int desiredCoord,
                       const int max) {
    if (desiredCoord >= 0 && desiredCoord < max)
        return desiredCoord;

    if (desiredCoord < 0)
        return max + desiredCoord;

    return desiredCoord % max;
}

// Checks if an image with one white pixel and the others black is convolved correctly.
template <typename PixelType, typename kType, size_t kWidth, size_t kHeight>
bool isOnePixelImageConvolved(const ofImage_<PixelType> &image, 
                              const Kernel<kType, kWidth, kHeight> &kernel, 
                              const size_t pixel_x, const size_t pixel_y) {
    const int startWidth = pixel_x - kWidth / 2;
    const int endWidth = pixel_x + kWidth / 2;

    const int startHeight = pixel_y - kHeight / 2;
    const int endHeight = pixel_y + kHeight / 2;

    for (int width = startWidth; width <= endWidth; width++)
        for (int height = startHeight; height <= endHeight; height++) {
            const size_t usedWidth = marginsHandling(width, image.getWidth());
            const size_t usedHeight = marginsHandling(height, image.getHeight());

            const ofColor_<PixelType> color = image.getColor(usedWidth, usedHeight);

            const float expectedColorValue = 255 * kernel[height - startHeight]
                                                         [width - startWidth];

            const ofColor_<PixelType> expectedColor(expectedColorValue,
                                                    expectedColorValue,
                                                    expectedColorValue);

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
bool areImagesEqual(const ofImage_<PixelType> &image_1, const ofImage_<PixelType> &image_2,
                    const float maxError) {
    if (image_1.getHeight() != image_2.getHeight() ||
        image_1.getWidth() != image_2.getWidth())
        return false;

    for (size_t width = 0; width < image_1.getWidth(); width++)
        for (size_t height = 0; height < image_1.getHeight(); height++)
            if ((image_1.getColor(width, height) - image_2.getColor(width, height)).
                    getBrightness() > maxError)
                return false;

    return true;
}

// Checks if the average brightness of two images is roughly the same.
template <typename PixelType>
bool hasSameBrightness(const ofImage &image_1, const ofImage_<PixelType> &image_2, 
                       const float maxError) {
    if (image_1.getHeight() != image_2.getHeight() ||
        image_1.getWidth() != image_2.getWidth())
            return false;

    // Brightness sum of all pixels in the images.
    unsigned long brightness_1 = 0;
    unsigned long brightness_2 = 0;

    for (size_t width = 0; width < image_1.getWidth(); width++)
        for (size_t height = 0; height < image_1.getHeight(); height++) {
            brightness_1 += image_1.getColor(width, height).getBrightness();
            brightness_2 += image_2.getColor(width, height).getBrightness();
        }

    size_t imageSize = image_1.getWidth() * image_1.getHeight();

    return std::abs(float((brightness_1 - brightness_2) / imageSize)) < maxError;
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

        const Kernel<float, 3, 3> kernel(
        {{
            {0.0625, 0.125, 0.0625},
            {0.125,  0.25,  0.125},
            {0.0625, 0.125, 0.0625},
        }});

        ofxTest(isOnePixelImageConvolved<unsigned char>(
                    convolve<float, 3, 3>
                        (getOnePixelImage<unsigned char>(101, 101, 50, 50), kernel),
                    kernel, 50, 50), 
                "center");

        ofxTest(isOnePixelImageConvolved<unsigned char>(
                    convolve<float, 3, 3>
                        (getOnePixelImage<unsigned char>(101, 101, 50, 0), kernel),
                    kernel, 50, 0), 
                "up");

        ofxTest(isOnePixelImageConvolved<unsigned char>(
                    convolve<float, 3, 3>
                        (getOnePixelImage<unsigned char>(101, 101, 50, 100), kernel),
                    kernel, 50, 100), 
                "down");

        ofxTest(isOnePixelImageConvolved<unsigned char>(
                    convolve<float, 3, 3>
                        (getOnePixelImage<unsigned char>(101, 101, 0, 50), kernel),
                    kernel, 0, 50), 
                "left");

        ofxTest(isOnePixelImageConvolved<unsigned char>(
                    convolve<float, 3, 3>
                        (getOnePixelImage<unsigned char>(101, 101, 100, 50), kernel),
                    kernel, 100, 50), 
                "right");

        ofImage image;
        image.load("test_img.bmp");

        convolve(image, kernel).save("ala.bmp");

        ofxTest(hasSameBrightness<unsigned char>
                    (image, convolve<float, 3, 3>(image, kernel), 0.00001),
                "brightness -> maxError = 0.00001");
    }

    void float_convolve_test() {
        ofLogNotice() << "Testing float_convolve()";

        const Kernel<float, 3, 3> kernel(
        {{
            {0.0625, 0.125, 0.0625},
            {0.125,  0.25,  0.125},
            {0.0625, 0.125, 0.0625},
        }});

        ofxTest(isOnePixelImageConvolved<float>(
                    float_convolve<float, 3, 3>
                        (getOnePixelImage<float>(101, 101, 50, 50), kernel),
                    kernel, 50, 50), 
                "center");

        ofxTest(isOnePixelImageConvolved<float>(
                    float_convolve<float, 3, 3>
                        (getOnePixelImage<float>(101, 101, 50, 0), kernel),
                    kernel, 50, 0), 
                "up");

        ofxTest(isOnePixelImageConvolved<float>(
                    float_convolve<float, 3, 3>
                        (getOnePixelImage<float>(101, 101, 50, 100), kernel),
                    kernel, 50, 100), 
                "down");

        ofxTest(isOnePixelImageConvolved<float>(
                    float_convolve<float, 3, 3>
                        (getOnePixelImage<float>(101, 101, 0, 50), kernel),
                    kernel, 0, 50), 
                "left");

        ofxTest(isOnePixelImageConvolved<float>(
                    float_convolve<float, 3, 3>
                        (getOnePixelImage<float>(101, 101, 100, 50), kernel),
                    kernel, 100, 50), 
                "right");

        ofImage image;
        image.load("test_img.bmp");

        ofxTest(hasSameBrightness<float>
                    (image, float_convolve<float, 3, 3>(image, kernel), 0.00001),
                "brightness -> maxError = 0.00001");
    }

    void iterative_convolve_test() {
        ofLogNotice() << "Testing iterativeConvolve()";

        const Kernel<float, 3, 3> kernel(
        {{
            {0.0625, 0.125, 0.0625},
            {0.125,  0.25,  0.125},
            {0.0625, 0.125, 0.0625},
        }});

        ofImage image;
        image.load("test_img.bmp");

        ofImage result = iterativeConvolve<float, 3, 3>(image, kernel, 5);

        ofFloatImage floatImage = float_convolve<float, 3, 3>(image, kernel);
        for (int times = 1; times < 5; times++)
            floatImage = 
                float_convolve<float, 3, 3>(convertToImage(floatImage), kernel);

        ofxTest(areImagesEqual(convertToImage(floatImage), result, 2),
                "5 times -> maxError = 2");
    }

    void run() {
        ofLogNotice() << '\n';
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
