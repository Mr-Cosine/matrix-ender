/*
 * Implementation
 */
#include "Useful.hpp"
#include "Rational.hpp"

template <>
int from_string<int>(std::string str) {
    return std::stoi(str);
}

template <>
long from_string<long>(std::string str) {
    return std::stol(str);
}

template <>
float from_string<float>(std::string str) {
    return std::stof(str);
}

template <>
double from_string<double>(std::string str) {
    return std::stod(str);
}

template <>
short from_string<short>(std::string str) {
    return static_cast<short>(std::stoi(str));
}

template <>
char from_string<char>(std::string str) {
    return static_cast<char>(std::stoi(str));
}

template <>
Rational from_string<Rational>(std::string str) {
    return Rational(str);
}