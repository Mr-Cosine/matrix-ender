#pragma once

#include <exception>
#include <concepts>
#include <string>
#include <vector>
#include <sstream>

#include "Useful.hpp"
#include "Matrix.hpp"
#include "Rational.hpp"

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
    // Default constructor (std::variant compatibility)
    Matrix() : row(0), col(0) {};

    // Filler constructor
    Matrix(long, long, T, FillType = FillType::EVERY);

    // String constructor
    Matrix(std::string descriptor, char row_delimiter = ';', char column_delimiter = ',')  {
        std::istringstream iss(descriptor);
        std::string row_tkn;
        while (std::getline(iss, row_tkn, row_delimiter) && !row_tkn.empty()) {
            std::vector<T> row;
            std::istringstream row_iss(row_tkn);
            std::string tkn;
            while (std::getline(row_iss, tkn, column_delimiter) && !tkn.empty()) {
                row.push_back(from_string<T>(tkn));
            }
            this->matrix.push_back(row);
        }
        this->row = this->matrix.size();
        this->col = this->row > 0 ? this->matrix[0].size() : 0;
    }

    // Insert value at [r,c] (in-place)
    Matrix<T> put(long, long, T);
    
    // Retrieve value from [r,c]
    T get(long, long) const;

    // Stringnify the matrix
    std::string toString(long row = -1, long column = -1, char row_delimiter = ';', char column_delimiter = ',', bool tab = true) const {
        if (row > this->row || column > this->col) {
            throw IndexOutOfBoundException("Index out of bounds");
        }

        row = row < 0 ? this->row : row;
        column = column < 0 ? this->col : col;
        std::ostringstream oss;
        oss << "[";
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < this->col; j++) {
                if (i != 0 && j == 0) oss << " ";
                oss << this->matrix[i][j];
                if (j != this->col - 1) {
                    oss << column_delimiter;
                    if (tab) oss << "\t";
                }
            }
            if (i != column - 1) oss << row_delimiter;
        }
        oss << "]" << std::endl;
        return oss.str();
    }
    
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


template <Arithmetic T>
std::ostream& operator<<(std::ostream& os, const Matrix<T> matrix) {
    return os << matrix.toString();
}