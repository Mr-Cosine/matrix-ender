/*
 * Set of useful vector utilities
 */

#include <vector>
#include <cmath>
#include <numeric>

#include "concepts.hpp"
#include "util.hpp"

template <Arithmetic T>
class matrix;

// Vectorized arithmetic
template <typename T>
requires Arithmetic<T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) return std::runtime_error("Dimension mismatch!");

    std::vector<T> resultant(a.size());
    for (int i = 0; i < a.size(); i++) {
        resultant[i] = a[i] + b[i];
    }

    return resultant;
}

template <typename T>
requires Arithmetic<T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) return std::runtime_error("Dimension mismatch!");

    std::vector<T> resultant(a.size());
    for (int i = 0; i < a.size(); i++) {
        resultant[i] = a[i] - b[i];
    }

    return resultant;
}

template <typename T>
requires Arithmetic<T>
std::vector<T> operator-=(const std::vector<T>& a, const std::vector<T>& b) {
    return a - b;
}

template <typename T>
requires Arithmetic<T>
std::vector<T> operator*(T scalar, const std::vector<T>& v) {
    std::vector<T> resultant(v.size());
    for (int i = 0; i < v.size(); i++) {
        resultant[i] = scalar * v[i];
    }

    return resultant;
}

template <typename T>
requires Arithmetic<T>
std::vector<T> operator/(const std::vector<T>& v, T scalar) {
    std::vector<T> resultant(v.size());
    for (int i = 0; i < v.size(); i++) {
        resultant[i] = v[i] / scalar;
    }

    return resultant;
}

// For type safety (Tho not necessary as C++ autocast integral to double)
template <typename T>
requires std::is_integral_v<T>
std::vector<double> toDouble(const std::vector<T>& v) {
    std::vector<double> resultant(v.size());
    std::transform(v.begin(), v.end(), resultant.begin(), [](T a) {
        return static_cast<double>(a);
    });
    return resultant;
}

template <typename T>
requires Ordered<T> && Arithmetic<T>
inline constexpr T dot_product(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) return std::runtime_error("Dimension mismatch!");

    T product(0);
    for (size_t i = 0; i < a.size(); i++) {
        product += a[i] * b[i];
    }

    return product;
}

template <typename T>
requires Ordered<T> && Arithmetic<T>
inline constexpr bool orthogonal(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) std::runtime_error("Dimension mismatch!");
    return dot_product(a, b) == 0;
}

template <typename T>
requires Ordered<T> && Arithmetic<T>
inline constexpr T norm(const std::vector<T>& v) {
    return sqrt(
        std::reduce(v.begin(), v.end(), 0, [](T a, T b) {
            return a + b*b;
        })
    );
}

template <typename T>
requires Ordered<T> && Arithmetic<T>
inline constexpr bool is_unit(const std::vector<T>& v) {
    return equals(norm(v), 1);
}

template <typename T>
requires Ordered<T> && Arithmetic<T>
inline constexpr bool unitize(const std::vector<T>& v) {
    return v / norm(v);
}

/**
 * Check if `a` is orthonormal (orthogonal unit vector) to `b`
 *
 * @param a Vector being checked
 * @param b Vector being compared (Doesn't have to be a unit vector)
 * 
 * @return Boolean
 */
template <typename T>
requires Ordered<T> && Arithmetic<T>
inline constexpr bool orthonormal(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) std::runtime_error("Dimension mismatch!");
    return orthogonal(a, b) && is_unit(a);
}

/**
 * Finds the projection of `v` onto `u`
 *
 * @param u Vector being transformed
 * @param v Vector being projected onto
 * 
 * @return proj_u_v
 */
template <typename T>
requires Ordered<T> && Arithmetic<T>
inline constexpr std::vector<T> project(const std::vector<T>& v, const std::vector<T>& u) {
    if (v.size() != u.size()) return std::runtime_error("Dimension mismatch!");
    T scale = dot_product(u, v) / dot_product(u, u);
    std::vector resultant = scale * u;
    return resultant;
}


/**
 * @brief The Grand Schmidt Process: https://en.wikipedia.org/wiki/Gram–Schmidt_process
 * 
 * @note The function does not modify the input matrix
 * 
 * The function transforms the input into an orthonormal matrix. The first vector will be used
 * as the basis. Zero vectors will be skipped. (TO BE IMPLEMENTED)
 * 
 * @param m The matrix to be Gram Schmidtized
 * 
 * @throws Throws `NonsquareMatrixException` if the input matrix is not a square matrix
 * 
 * @returns An orthonormal matrix
 */
template <typename T>
requires Ordered<T> && Arithmetic<T>
inline matrix<T> gram_schmidtize(const matrix<T>& m, bool tranpose_me = false) {
    std::vector<size_t> ps{};
    matrix<T> res(m.row, m.col);
    for (int i = 0; i < m.row; i++) {
        if (is_zero(m.data[i])) {
            res.append_row(m.data[i]);
            continue;
        }

        // Basis logic
        if (ps.empty()) {
            ps.push_back(i);
            res.append_row(unitize(m.data[i]));
        }
        // Transform logic
        else {
            std::vector<T> current = m.data[i];
            for (const size_t idx: ps) {
                current -= project(current, m.data[i]);
            }
            res.append_row(unitize(current));
            ps.push_back(i);
        }
    }
    return res;
}
