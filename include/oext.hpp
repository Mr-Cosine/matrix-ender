/*
 * ostream extension
 * 
 * Custom class print logic
 */

#pragma once
#include "rational.hpp"
#include "matrix.hpp"
#include "CLI.hpp"

inline std::ostream& operator<<(std::ostream& os, const std::monostate& _) {
    (void)_;
    return os << "<void>";
}

inline std::ostream& operator<<(std::ostream& os, const Variable::VarType& type) {
    switch (type) {
        case Variable::VarType::MATRIX:
            return os << "[Matrix]";
        case Variable::VarType::PRIMITIVE:
            return os << "[Primitive]";
        default:
            return os << "[VOID]";
    }
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector) {
    os << "[";
    for (size_t i = 0; i < vector.size(); i++) {
        if (i != 0) os << " ";
        os << vector;
        if (i != vector.size() - 1) os << ";\n";
    }
    return os << "]";
}

inline std::ostream& operator<<(std::ostream& os, const Variable::ExactType& type) {
    switch (type) {
        case Variable::ExactType::NUMBER:
            return os << "<number>";
        case Variable::ExactType::DECIMAL:
            return os << "<decimal>";
        case Variable::ExactType::RATIONAL:
            return os << "<rational>";
        case Variable::ExactType::STRING:
            return os << "<string>";
        case Variable::ExactType::VOID:
            return os << "<void>";
    }
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const matrix<T>& mat) {
    return os << mat.toString();
}

inline std::ostream& operator<<(std::ostream& os, const rational& rational) {
    return os << rational.toString();
}