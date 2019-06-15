#pragma once

#include "ofImage.h"
#include "kernel.h"

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
ofImage convolve(const ofImage& input, const Kernel &kernel);

/**
 * @brief Applies convolution operations iteratively on an image. 
 *
 * @param input -> The image to be used.
 * @param kernel -> The kernel to be used.
 * @param times -> The number of convolve operations to be applied.
 */
ofImage iterativeConvolve(const ofImage& image, const Kernel &kernel,
                          const int times);
