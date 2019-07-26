#pragma once

#include "ofImage.h"

#include "kernel.h"

/**
 * @brief Applies a glow effect on an image by drawing
 *        the blurred image over the given one.
 *
 * @param image -> The image to be glowing.
 * @param kernel -> The kernel to be used to blur.
 */
ofImage glow(const ofImage &image, const Kernel &kernel);
