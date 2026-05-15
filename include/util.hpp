/*
 * Useful helper functions
 */

#pragma once
#include "initializer_list"
#include <iostream>
#include <concepts>
#include <rational.hpp>
#include <vector>
#include <string>

/**
 * Parse to number type from string
 */

 // Forward declarations
template <Arithmetic T>
class Vector;


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
inline long double from_string<long double>(std::string str) {
    return std::stold(str);
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
inline rational from_string<rational>(std::string str) {
    return rational(str);
}

/**
 * Utility functions
 * 
 * The following function's definition may be requested at compile time
 * Thus must be kept in header
 */
// Bidirectional trimming
inline std::string trim(const std::string& str) {
    if (str.size() == 0) return str;

    size_t bgn, end;
    for (bgn = 0; std::isspace(str[bgn]); bgn++);
    for (end = str.size() - 1; std::isspace(str[end]); end--);
    return str.substr(bgn, end - bgn + 1);
}

// Left trimming
inline std::string ltrim(const std::string& str) {
    size_t bgn;
    for (bgn = 0; std::isspace(str[bgn]); bgn++);
    return str.substr(bgn, str.size() - bgn);
}

// Right trimming
inline std::string rtrim(const std::string& str) {
    size_t end;
    for (end = str.size() - 1; std::isspace(str[end]); end--);
    return str.substr(0, end + 1);
}

inline std::vector<std::string> split(const std::string& str, char delimiter) {
    std::istringstream iss(str);
    std::vector<std::string> ss;
    std::string cur{};
    while (std::getline(iss, cur, delimiter)) {
        ss.push_back(cur);
    }
    return ss;
}

inline std::vector<std::string> split(const std::string& str, char delimiter, int max_split) {
    std::istringstream iss(str);
    std::vector<std::string> ss;
    std::string cur{};
    int splitted = 0;
    while (splitted < max_split && std::getline(iss, cur, delimiter)) {
        ss.push_back(cur);
        splitted++;
    }

    if (std::getline(iss, cur)) {
        ss.push_back(cur);
    }

    return ss;
}

// Global removal
inline std::string removeAll(std::string str, char which) {
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == which) str.erase(i, 1);
    }
    return str;
};

inline size_t count(const std::string& haystack, char needle) {
    size_t count = 0;
    for (const char c: haystack) {
        if (c == needle) count++;
    }
    return count;
}

// Greedy search
inline std::string delimitBy(const std::string& str, char from, char to) {
    size_t start = str.find_first_of(from);
    size_t end = str.find_last_of(to);
    return str.substr(start + 1, end - start);
}

// Join
inline std::string join(std::vector<std::string> tokens, char delimiter) {
    std::ostringstream oss;
    for (size_t i = 0; i < tokens.size(); i++) {
        oss << tokens[i];
        if (i != tokens.size() - 1) oss << delimiter;
    }
    return oss.str();
}

// Contains
inline bool contains(const std::string& haystack, const char needle) {
    for (const char c: haystack) {
        if (c == needle) return true;
    }
    return false;
}

// Contains any
inline bool containsAny(const std::string& haystack, const std::string& needles) {
    for (const char c: needles) {
        if (contains(haystack, c)) return true;
    }
    return false;
}

inline bool isNumeric(const std::string& str) {
    if (count(str, '.') > 1) return false;
    for (const char c: str) {
        if (c - '0' > 9 || c - '0' < 0) {
            return false;
        }
    }
    return true;
}

inline bool isDecimal(const std::string& str) {
    return isNumeric(str) && count(str, '.') == 1;
}

inline bool isInteger(const std::string& str) {
    return isNumeric(str) && count(str, '.') == 0;
}

template <typename T, typename... Args>
requires ((std::is_integral_v<Args> || std::is_floating_point_v<Args>) && ...)
inline constexpr auto min(T first, Args... args) {
    T result = first;
    ((result = result < args ? result : args), ...);
    return result;
}

template <Ordered T>
inline constexpr bool is_zero(T a);

template <>
inline constexpr bool is_zero<char>(char c) {
    return c == 0;
}

template <>
inline constexpr bool is_zero<short>(short v) {
    return v == 0;
}

template <>
inline constexpr bool is_zero<int>(int v) {
    return v == 0;
}

template <>
inline constexpr bool is_zero<long>(long v) {
    return v == 0;
}

template <>
inline constexpr bool is_zero<float>(float v) {
    return std::abs(v) < 1e-6;
}

template <>
inline constexpr bool is_zero<double>(double v) {
    return std::abs(v) < 1e-6;
}

template <Ordered T>
inline constexpr bool is_zero(Vector<T> v) {
    return std::all_of(v.begin(), v.end(), [](T a) {
        return is_zero(a);
    });
}

template <>
inline bool is_zero<rational>(rational r) {
    return r.num == 0;
}

template <Ordered T>
inline constexpr bool equals(T, T);

template <>
inline constexpr bool equals<char>(char value, char target) {
    return value == target;
}

template <>
inline constexpr bool equals<short>(short value, short target) {
    return value == target;
}

template <>
inline constexpr bool equals<int>(int value, int target) {
    return value == target;
}

template <>
inline constexpr bool equals<long>(long value, long target) {
    return value == target;
}

template <>
inline constexpr bool equals<float>(float value, float target) {
    return std::abs(value - target) < 1e-6;
}

template <>
inline constexpr bool equals<double>(double value, double target) {
    return std::abs(value - target) < 1e-6;
}

template <>
inline constexpr bool equals<rational>(rational value, rational target) {
    return value.num - target.num == 0;
}

// Colorful terminal
#ifdef _WIN32
#include <windows.h>
inline void err(const std::string& message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cout << message << std::endl;
    SetConsoleTextAttribute(hConsole, 7); // reset
}
#else
inline void err(const std::string& message) {
    std::cout << "\033[31m" << message << "\033[0m" << std::endl;
}
#endif