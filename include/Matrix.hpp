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
    LOWER_TRI,
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

public:
    // Filler constructor
    Matrix(long, long, T, FillType = FillType::EVERY);

    // String constructor
    Matrix(std::string, char = ';', char = ',');

    // Move and destructor: use default
    Matrix(Matrix<T>&&) = default;
    ~Matrix() = default;

    // Insert value at [r,c] (in-place)
    Matrix<T> put(long, long, T);
    
    // Retrieve value from [r,c]
    T get(long, long) const;

    // Stringnify the matrix
    std::string toString(long = -1, long = -1, char = ';', char = ',') const;
    
    // Print the matrix
    void print(char = '\n', char = ',') const;

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
    Matrix<T> ref();

    // get reduced row echelon form
    Matrix<T> rref();

    // Get determinant
    T det();

    // Get transpose
    Matrix<T> transpose();

    // Get inverse (throws NotInvertibleMatrixException if not invertible)
    Matrix<T> inverse();

    // Get eigenvalues
    Vector eigenval();

    // Get eigenvectors
    std::vector<Vector> eigenvec();

    // Check if a vector is in the matrix's spanning space
    // FROM [//determine if a vector b is within the span(in column space/ being the linear combination) of matrix]
    bool inspan(Vector);

    // Determine type of solution(unique, infinite, nil) & solve the matrix with the input vector b
    // Reading from Solution.vector when Solution.type = infinite / nil is undefined behavior
    Solution solve(Vector);
    
    // Diagnoize the matrix
    Matrix<T> diagonize();

    // LU Factorization
    Matrix<T> LU();

    // Get the norm(1-norm, infinity norm, euclidean norm) of a matrix
    long norm(std::string);

    // Matrix properties

    // Get rank of matrix
    long rank();

    // Get nullity of matrix                                              
    long null();

    // Get dimension of matrix
    long dim();
};
