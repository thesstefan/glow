#pragma once

#include "ofImage.h"
#include "kernel.h"

/** 
 * @brief Converts an ofImage_ to an ofImage.
 *
 * @warning Accuracy can be lost as PixelType to unsigned char
 *          conversions are made.
 */
template <typename PixelType>
ofImage convertToImage(const ofImage_<PixelType> &input);

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
 * @brief Convolves an image with a Kernel using unsigned char values. 
 *
 * For succesive calls, iterativeConvolve is recommended for
 * better accuracy.
 */
template <typename PixelType>
ofImage convolve(const ofImage_<PixelType> &input, 
                 const Kernel &kernel);

/**
 * @brief Applies convolution operations iteratively on an image. 
 *
 * Accuracy is manitained by using float_convolve.
 *
 * @param input -> The image to be used.
 * @param kernel -> The kernel to be used.
 * @param times -> The number of convolve operations to be applied.
 */
template <typename PixelType>
ofImage iterativeConvolve(const ofImage_<PixelType> &image, 
                          const Kernel &kernel,
                          const int times);
