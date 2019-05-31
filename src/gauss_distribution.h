#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <stdexcept>

/**
 * @brief Returns the probability of getting a random variable having 
 *        the given value from a normal distribution, given its mean and variance.
 *
 * @param value -> The desired value of the random variable.
 *
 * @param mean -> The mean of the distribution.
 * @param variance -> The variance of the distribution.
 *
 * @warning variance must be positive. std::invalid_argument is thrown otherwise.
 */
float gaussianProbabilty(const float value, const float mean, const float variance) {
    if (variance <= 0)
        throw std::invalid_argument("std::invalid_argument: gaussianProbabilty() -> variance must be greater than 0.");
    
    const float multiplier = 1. / std::sqrt(2 * M_PI * variance);
    const float power = -((value - mean) * (value - mean)) / (2 * variance);

    return multiplier * std::pow(M_E, power);
}
