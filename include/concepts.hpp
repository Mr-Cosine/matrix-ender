/*
 * Concept definitions
 */

#pragma once

#include <concepts>

template <typename T>
concept Arithmetic = requires(T a, T b) {
    a + b;
    a - b;
    a * b;
    a / b;
};