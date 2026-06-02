#pragma once

#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>

inline int ToInt(std::string_view value) {
    while (!value.empty() && std::isspace(static_cast<unsigned char>(value.front()))) {
        value.remove_prefix(1);
    }

    while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back()))) {
        value.remove_suffix(1);
    }

    int result;

    const char *begin = value.data();
    const char *end = begin + value.size();

    auto [ptr, ec] = std::from_chars(begin, end, result);

    if (ec != std::errc{} || ptr != end) {
        throw std::invalid_argument("Cannot convert '" + std::string(value) + "' to int");
    }

    return result;
}
