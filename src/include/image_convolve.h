#pragma once

#include "ofImage.h"
#include "kernel.h"

/** 
 * @brief Converts an ofFloatImage to an ofImage.
 *
 * @warning Accuracy can be lost as float to unsigned char
 *          conversions are made.
 */
ofImage convertToImage(const ofFloatImage &input);

/**
 * @brief Convolve an ofImage with a Kernel using float values.
 *
 * This is more accuarate than convolve and it's recommended to
 * be used while dealing with successive convolutions.
 */
ofFloatImage float_convolve(const ofImage &input,
                            const Kernel &kernel);

/**
 * @brief Convolves an ofImage with a Kernel using unsigned char values. 
 *
 * For succesive calls, iterativeConvolve is recommended for
 * better accuracy.
 */
ofImage convolve(const ofImage &input, 
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
ofImage iterativeConvolve(const ofImage &image, 
                          const Kernel &kernel,
                          const int times);
