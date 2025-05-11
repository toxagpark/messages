#ifndef LINK_GENERATE_HPP
#define LINK_GENERATE_HPP

#include <iostream>
#include <string>
#include <random>

std::string GenerateRandomLink(size_t length) {
    static const std::string chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> distribution(0, chars.size() - 1);

    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        result += chars[distribution(generator)];
    }

    return result;
}

#endif LINK_GENERATE_HPP