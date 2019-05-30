#pragma once

#define _USE_MATH_DEFINES

#include <cmath>

/**
 * @brief Returns the probability of getting a random variable having 
 *        the given value from a normal distribution, given its mean and variance.
 *
 * @param value -> The desired value of the random variable.
 *
 * @param mean -> The mean of the distribution.
 * @param variance -> The variance of the distribution.
 */
float gaussianProbabilty(float value, float mean, float variance) {
    const float multiplier = 1. / std::sqrt(2 * M_PI * variance);
    const float power = -((value - mean) * (value - mean)) / (2 * variance);

    return multiplier * std::pow(M_E, power);
}
