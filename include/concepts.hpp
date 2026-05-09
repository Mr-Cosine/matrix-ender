/*
 * Concept definitions
 */

#pragma once

#include <rational.hpp>
#include <concepts>
#include <type_traits>

template <typename T>
concept Arithmetic = requires(T a, T b) {
    a + b;
    a - b;
    a * b;
    a / b;
};

template <typename T>
concept Ordered = requires(T a, T b) {
    { a < b }  -> std::same_as<bool>;
    { a > b }  -> std::same_as<bool>;
    { a == b } -> std::same_as<bool>;
    { a != b } -> std::same_as<bool>;
};