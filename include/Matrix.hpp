#pragma once

#include <exception>
#include <concepts>
#include <string>
#include <vector>

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
class Matrix {
private:
    long row;
    long col;
    std::vector<std::vector<T>> matrix;
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
    Matrix(long, long, T, FillType = FillType::EVERY);

    // String constructor
    Matrix(std::string, char = ';', char = ',');


    // Insert value at [r,c] (in-place)
    Matrix<T> put(long, long, T);
    
    // Retrieve value from [r,c]
    T get(long, long) const;

    // Stringnify the matrix
    std::string toString(long = -1, long = -1, char = '\n', char = ',', bool = true) const;
    
    // Print the matrix
    void print(char = '\n', char = ',', bool = true, bool = true) const;

    // Matrix arithmetics (symbolic)

    Matrix<T> operator+(const Matrix<T>&) const;
    Matrix<T> operator-(const Matrix<T>&) const;
    Matrix<T> operator*(const Matrix<T>&) const;
    Matrix<T> operator/(const Matrix<T>&) const;

    // Matrix arithmetics (method invoc.)

    Matrix<T> add(const Matrix<T>&) const;
    Matrix<T> sub(const Matrix<T>&) const;
    Matrix<T> mult(const Matrix<T>&) const;
    Matrix<T> div(const Matrix<T>&) const;

    // Matrix operations

    // Row operation (in-place)
    void ro(int, int, int, int);

    // Column operation (in-place)
    void co(int, int, int, int);

    // Row exchange (in-place)
    void re(int, int);

    // Column exchange (in-place)
    void ce(int, int);

    // get row echelon form
    Matrix<T> ref() const;

    // get reduced row echelon form
    Matrix<T> rref() const;

    // Get determinant
    T det() const;

    // Get transpose
    Matrix<T> transpose() const;

    // Get inverse (throws NotInvertibleMatrixException if not invertible)
    Matrix<T> inverse() const;

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
