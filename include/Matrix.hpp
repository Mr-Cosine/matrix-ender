#pragma once

#include <exception>
#include <concepts>
#include <string>
#include <vector>
#include "rational.hpp"

template <typename T>
concept Arithmetic = requires(T a, T b) {
    a + b;
    a - b;
    a * b;
    a / b;
};

enum class FillType {
    UPPER_TRI,
    UPPER_TRI_R,
    LOWER_TRI,
    LOWER_TRI_R,
    EVERY
};

template <Arithmetic T>
class matrix {
private:
    long row;
    long col;
    std::vector<std::vector<T>> data;
    using Vector = std::vector<T>;

    // Matrix Solution Result Type
    struct Solution {
    private:
        enum class SolutionType {
            NIL,
            UNIQUE,
            INFINITE
        };

    public:
        Vector vector;
        SolutionType type;
    };

    // Matrix Exception Types
    class NotInvertibleMatrixException: public std::exception {
    private:
        std::string message;

    public:
        explicit NotInvertibleMatrixException(std::string message) : message(message) {}

        const char* what() const noexcept override {
            return this->message.c_str();
        }
    };

    class InvalidFillTypeException: public std::exception {
    private:
        std::string message;
    public:
        explicit InvalidFillTypeException(std::string message) : message(message) {}

        const char* what() const noexcept override {
            return this->message.c_str();
        }
    };

    class IndexOutOfBoundException: public std::exception {
    private:
        std::string message;
    public:
        explicit IndexOutOfBoundException(std::string message) : message(message) {}

        const char* what() const noexcept override {
            return this->message.c_str();
        }
    };

public:
    // Filler constructor
    matrix(long, long, T, FillType = FillType::EVERY);

    // String constructor
    matrix(std::string, char = ';', char = ',');


    // Insert value at [r,c] (in-place)
    void put(long, long, T);
    
    // Retrieve value from [r,c]
    T get(long, long) const;

    // Stringnify the matrix
    std::string toString(long = -1, long = -1, char = '\n', char = ',', bool = true) const;
    
    // Print the matrix
    void print(char = '\n', char = ',', bool = true, bool = true) const;

    // Matrix arithmetics (symbolic)

    matrix<T> operator+(const matrix<T>&) const;
    matrix<T> operator-(const matrix<T>&) const;
    matrix<T> operator*(const matrix<T>&) const;
    matrix<T> operator*(const T) const;
    matrix<T> operator/(const matrix<T>&) const;

    matrix<T>& matrix<T>::operator+=(const matrix<T>& other);
    matrix<T>& matrix<T>::operator-=(const matrix<T>& other);
    matrix<T>& matrix<T>::operator*=(T scalar);

    // Matrix arithmetics (method invoc.)

    matrix<T> add(const matrix<T>&) const;
    matrix<T> sub(const matrix<T>&) const;
    matrix<T> mult(const matrix<T>&) const;
    matrix<T> div(const matrix<T>&) const;

    // Matrix operations

    // Row operation (in-place)
    void ro(long, long, long, long);

    // Column operation (in-place)
    void co(long, long, long, long);

    // Row exchange (in-place)
    void re(long, long);

    // Column exchange (in-place)
    void ce(long, long);

    // get row echelon form
    matrix<T> ref() const;
    matrix<T> ref(long) const;

    // get reduced row echelon form
    matrix<T> rref() const;
    matrix<T> rref(long) const;

    // Get determinant
    T det() const;

    // Get transpose
    matrix<T> transpose() const;

    // Get inverse (throws NotInvertibleMatrixException if not invertible)
    matrix<T> inverse() const;

    // Get eigenvalues
    Vector eigenval() const;

    // Get eigenvectors
    std::vector<Vector> eigenvec() const;

    // Check if a vector is in the matrix's spanning space
    // FROM [//determine if a vector b is within the span(in column space/ being the linear combination) of matrix]
    bool inspan(Vector) const;

    // Determine type of solution(unique, infinite, nil) & solve the matrix with the input vector b
    // Reading from Solution.vector when Solution.type = infinite / nil is undefined behavior
    Solution solve(Vector) const;
    
    // Diagnoize the matrix
    Matrix<T> diagonize() const;

    // LU Factorization
    Matrix<T> LU() const;

    // Get the norm(1-norm, infinity norm, euclidean norm) of a matrix
    long norm(std::string) const;

    // Matrix properties

    // Get rank of matrix
    long rank() const;

    // Get nullity of matrix                                              
    long null() const;

    // Get dimension of matrix
    long dim() const;
};
