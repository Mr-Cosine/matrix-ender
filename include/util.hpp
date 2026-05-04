#pragma once
#include "Rational.hpp"

template <typename T>
T from_string(std::string);

template <>
inline int from_string<int>(std::string str) {
    return std::stoi(str);
}

template <>
inline long from_string<long>(std::string str) {
    return std::stol(str);
}

template <>
inline float from_string<float>(std::string str) {
    return std::stof(str);
}

template <>
inline double from_string<double>(std::string str) {
    return std::stod(str);
}

template <>
inline short from_string<short>(std::string str) {
    return static_cast<short>(std::stoi(str));
}

template <>
inline char from_string<char>(std::string str) {
    return static_cast<char>(std::stoi(str));
}

template <>
inline Rational from_string<Rational>(std::string str) {
    return Rational(str);
}