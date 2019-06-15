#pragma once

#include "ofImage.h"
#include "kernel.h"

/** 
 * @brief Converts an ofFloatImage to an ofImage.
 *
 * @warning Accuracy can be lost.
 */
ofImage convertFromFloat(const ofFloatImage &floatImage);

/**
 * @brief Convolve an image with a Kernel using float values.
 *
 * This is more accuarate than convolve and it's recommended to
 * be used while dealing with successive convolutions.
 */
template <typename PixelType>
ofFloatImage float_convolve(const ofImage_<PixelType> &input,
                            const Kernel &kernel);
/**
 * @brief Convolves an image with a Kernel. 
 *
 * Used to blur an image by using a gaussian Kernel.
 *
 * @param input -> The image to be used.
 * @param kernel -> The kernel to be used.
 *
 * For succesive calls, iterativeConvolve is recommended for
 * better accuracy.
 */
ofImage convolve(const ofImage &input, const Kernel &kernel);

/**
 * @brief Applies convolution operations iteratively on an image. 
 *
 * @param input -> The image to be used.
 * @param kernel -> The kernel to be used.
 * @param times -> The number of convolve operations to be applied.
 */
ofImage iterativeConvolve(const ofImage &image, const Kernel &kernel,
                          const int times);

