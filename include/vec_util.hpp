/*
 * Set of useful vector utilities
 */
#pragma once

#include <vector>
#include <cmath>
#include <numeric>

#include "concepts.hpp"
#include "util.hpp"


template <Arithmetic T>
class matrix;

template <Arithmetic T>
class Vector {
private:
    std::vector<T> data;

public:
    Vector(std::initializer_list<T> list): data(list) {}
    Vector(size_t dimension, T filler): data(dimension, filler) {}
    Vector(size_t dimension): data(dimension) {}
    Vector(): data() {}
    
    typename std::vector<T>::iterator begin() { return this->data.begin(); }
    typename std::vector<T>::iterator end() { return this->data.end(); }
    typename std::vector<T>::const_iterator cbegin() const { return this->data.cbegin(); }
    typename std::vector<T>::const_iterator cend() const { return this->data.cend(); }
    size_t size() const { return this->data.size(); }

    T& operator[](size_t i) { return this->data[i]; }
    const T& operator[](size_t i) const { return this->data[i]; }

    Vector operator+(const std::vector<T>& other) const {
        if (this->size() != other.size()) std::runtime_error("Dimension mismatch!");

        Vector<T> resultant(this->size());
        for (size_t i = 0; i < this->size(); i++) {
            resultant[i] = (*this)[i] + other[i];
        }

        return resultant;
    }

    Vector<T>& operator+=(const Vector<T>& other) {
        *this = *this + other;
        return *this;
    }

    Vector<T> operator-(const Vector<T>& other) const {
        if (this->size() != other.size()) std::runtime_error("Dimension mismatch!");

        Vector<T> resultant(this->size());
        for (size_t i = 0; i < this->size(); i++) {
            resultant[i] = (*this)[i] - other[i];
        }

        return resultant;
    }

    Vector<T>& operator-=(const Vector& other) {
        *this = *this - other;
        return *this;
    }

    Vector operator*(T scalar) const {
        Vector<T> resultant(this->size());
        for (size_t i = 0; i < this->size(); i++) {
            resultant[i] = scalar * (*this)[i];
        }

        return resultant;
    }

    Vector<T> operator/(T scalar) const {
        Vector<T> resultant(this->size());
        for (size_t i = 0; i < this->size(); i++) {
            resultant[i] = (*this)[i] / scalar;
        }

        return resultant;
    }

    constexpr T norm() const {
        return sqrt(
            std::reduce(this->cbegin(), this->cend(), T{0}, [](T a, T b) {
                return a + b * b;
            })
        );
    }


    /**
     * Finds the projection of `v` onto `u`
     *
     * @param u Vector being transformed
     * @param v Vector being projected onto
     * 
     * @return proj_u_v
     */
    Vector<T> project(const Vector<T>& u) const {
        if (this->size() != u.size()) std::runtime_error("Dimension mismatch!");
        T scale = dot_product(u, *this) / dot_product(u, u);
        Vector<T> resultant = u * scale;
        return resultant;
    }

    bool unit() const {
        return equals(this->norm(), 1);
    }

    Vector<T> unitize() const {
        return *this / this->norm();
    }

    Vector<T> round(size_t toFixed) const {
        Vector<T> resultant(this->size());
        std::transform(this->cbegin(), this->cend(), resultant.begin(), [&toFixed](T value) {
            return static_cast<T>(
                static_cast<double>(std::round(value * std::pow(10, toFixed)))
                / std::pow(10, toFixed)
            );
        });
        return resultant;
    }
};

template <Arithmetic T>
class VectorPack {
private:
    std::vector<Vector<T>> data;
public:
    VectorPack(std::initializer_list<Vector<T>> list) : data(list) {}
    VectorPack(size_t size, size_t inner_size) : data(size, Vector<T>(inner_size)) {}
    VectorPack(size_t count) : data(count) {}
    VectorPack() : data() {}

    typename std::vector<Vector<T>>::iterator begin() { return this->data.begin(); }
    typename std::vector<Vector<T>>::iterator end() { return this->data.end(); }
    typename std::vector<Vector<T>>::const_iterator cbegin() { return this->data.cbegin(); }
    typename std::vector<Vector<T>>::const_iterator cend() { return this->data.cend(); }

    Vector<T>& operator[](size_t i) { return data[i]; }
    const Vector<T>& operator[](size_t i) const { return data[i]; }
    constexpr size_t size() const { return this->data.size(); }
    constexpr bool empty() const { return this->data.empty(); }

    void append(const Vector<T>& v) {
        this->data.push_back(v);
    }

    void remove(size_t index) {
        if (index >= this->data.size()) std::runtime_error("Index out of bound");
        this->data.erase(this->data.begin() + index);
    }
};


// For type safety (Tho not necessary as C++ autocast integral to double)
template <typename T>
requires std::is_integral_v<T>
Vector<double> toDouble(const Vector<T>& v) {
    Vector<double> resultant(v.size());
    std::transform(v.cbegin(), v.cend(), resultant.begin(), [](T a) {
        return static_cast<double>(a);
    });
    return resultant;
}

template <typename T>
requires Ordered<T> && Arithmetic<T>
inline constexpr T dot_product(const Vector<T>& a, const Vector<T>& b) {
    if (a.size() != b.size()) std::runtime_error("Dimension mismatch!");

    T product(0);
    for (size_t i = 0; i < a.size(); i++) {
        product += a[i] * b[i];
    }

    return product;
}

template <typename T>
requires Ordered<T> && Arithmetic<T>
inline constexpr bool orthogonal(const Vector<T>& a, const Vector<T>& b) {
    if (a.size() != b.size()) std::runtime_error("Dimension mismatch!");
    return dot_product(a, b) == 0;
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
inline constexpr bool orthonormal(const Vector<T>& a, const Vector<T>& b) {
    if (a.size() != b.size()) std::runtime_error("Dimension mismatch!");
    return orthogonal(a, b) && a.unit();
}

template <Arithmetic T>
inline std::ostream& operator<<(std::ostream& os, const Vector<T>& vector) {
    os << "[";
    for (size_t i = 0; i < vector.size(); i++) {
        os << vector[i];
        if (i != vector.size() - 1) os << ",";
    }
    return os << "]";
}

/**
 * @brief The Gram Schmidt Process: https://en.wikipedia.org/wiki/Gram–Schmidt_process
 * 
 * @note The function does not modify the input matrix
 * 
 * The function transforms the input into an orthonormal matrix. The first vector will be used
 * as the basis. Zero vectors will be skipped. (TO BE IMPLEMENTED)
 * 
 * @param m The `VectorPack` to be Gram Schmidtized
 * 
 * @throws Throws `NonsquareMatrixException` if the input matrix is not a square matrix
 * 
 * @returns An orthonormal `VectorPack`
 */
template <typename T>
requires Ordered<T> && Arithmetic<T>
inline VectorPack<T> gram_schmidtize(const VectorPack<T>& pack) {
    if (pack.empty()) return pack;

    VectorPack<T> res;
    for (size_t i = 0; i < pack.size(); i++) {
        if (is_zero<T>(pack[i])) {
            res.append(pack[i]);
            continue;
        }

        // Basis logic
        if (res.empty()) {
            res.append(pack[i].unitize().round(3));
        }
        // Transform logic
        else {
            Vector<T> current = pack[i];
            for (const Vector<T>& v: res) {
                current -= pack[i].project(v);
            }
            res.append(
                is_zero<T>(current.norm())
                ? Vector<T>(current.size(), 0)
                : current.unitize().round(3)
            );
        }
    }
    return res;
}


