/*
 * The matrix interface
 */

#pragma once

#include <exception>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

#include "rational.hpp"
#include "util.hpp"
#include "concepts.hpp"

enum class FillType {
    UPPER_TRI,
    UPPER_TRI_R,
    LOWER_TRI,
    LOWER_TRI_R,
    EVERY,
    DIAGONAL
};

// Matrix Solution Result Type
template <Arithmetic T>
struct Solution {
    enum class SolutionType {
        NIL,
        UNIQUE,
        INFINITE
    };

    std::vector<std::vector<T>> vector_group;
    SolutionType type;
};

template <Arithmetic T>
class matrix {
private:
    long row;
    long col;
    std::vector<std::vector<T>> data;
    using Vector = std::vector<T>;

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

    class MalformedMatrixException: public std::exception {
    private:
        std::string message;
    public:
        explicit MalformedMatrixException(std::string message) : message(message) {}

        const char* what() const noexcept override {
            return this->message.c_str();
        }
    };

    class ComputationFailedException: public std::exception {
    private:
        std::string message;
    public:
        explicit ComputationFailedException(std::string message) : message(message) {}

        const char* what() const noexcept override {
            return this->message.c_str();
        }
    };

    class InvalidMatricesPairException: public std::exception {
    private:
        std::string message;
    public:
        explicit InvalidMatricesPairException(std::string message) : message(message) {}

        const char* what() const noexcept override {
            return this->message.c_str();
        }
    };


public:
    // Default constructor (std::variant compatibility)
    matrix() : row(0), col(0) {};

    // Constructor to define the size
    matrix(long r, long c) : row(r), col(c) {};

    // Filler constructor
    matrix(long, long, T, FillType = FillType::EVERY);

    //Constructor from a vector
    matrix(const Vector& vector) :
        row(vector.size()),
        col(1),
        data(row, std::vector<T>(col))
    {
        for (int r = 0; r < vector.size(); r++) {
            data[r][0] = vector[r];
        }
    }

    // String constructor
    matrix(std::string descriptor, char row_delimiter = ';', char column_delimiter = ',')  {
        if (descriptor.find('[') != std::string::npos && descriptor.find(']') != std::string::npos) {
            descriptor = descriptor.substr(descriptor.find('[') + 1, descriptor.find(']') - descriptor.find('[') - 1);
        } else if (descriptor.find('[') != std::string::npos && descriptor.find(']') != std::string::npos) {
            throw MalformedMatrixException("[ was never closed");
        }

        std::istringstream iss(descriptor);
        std::string row_tkn;
        while (std::getline(iss, row_tkn, row_delimiter) && !row_tkn.empty()) {
            std::vector<T> row;
            std::istringstream row_iss(row_tkn);
            std::string tkn;
            while (std::getline(row_iss, tkn, column_delimiter) && !tkn.empty()) {
                row.push_back(from_string<T>(tkn));
            }
            this->data.push_back(row);
        }
        this->row = this->data.size();
        this->col = this->row > 0 ? this->data[0].size() : 0;
    }

    // Insert value at [r,c] (in-place)
    void put(long, long, T);
    
    // Retrieve value from [r,c]
    T get(long, long) const;

    // Stringnify the matrix
    std::string toString(long row = -1, long column = -1, char row_delimiter = ';', char column_delimiter = ',', bool tab = true, bool line_break = true) const {
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
                oss << this->data[i][j];
                if (j != this->col - 1) {
                    oss << column_delimiter;
                    if (tab) oss << "\t";
                }
            }
            if (i != row - 1) {
                oss << row_delimiter;
                if (line_break) oss << "\n";
            }
        }
        oss << "]" << std::endl;
        return oss.str();
    }
    
    // Print the matrix
    void print(char = '\n', char = ',', bool = true, bool = true) const;

    // Matrix arithmetics (symbolic)

    matrix<T> operator+(const matrix<T>&) const;
    matrix<T> operator-(const matrix<T>&) const;
    matrix<T> operator*(const matrix<T>&) const;
    matrix<T> operator*(const T) const;
    matrix<T> operator/(const matrix<T>&) const;

    matrix<T>& operator+=(const matrix<T>&);
    matrix<T>& operator-=(const matrix<T>&);
    matrix<T>& operator*=(T);

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
    matrix<T> ref(long) const; //echelon but up until a column, not to end

    // get reduced row echelon form
    matrix<T> rref() const;
    matrix<T> rref(long) const; //rref but up until a column, not to end

    // Get determinant
    T det() const;

    // Get transpose
    matrix<T> transpose() const;

    // Get inverse (throws NotInvertibleMatrixException if not invertible)
    matrix<T> inverse() const;

    // Check if a vector is in the matrix's spanning space
    // FROM [//determine if a vector b is within the span(in column space/ being the linear combination) of matrix]
    bool inspan(Vector) const;

    // Determine type of solution(unique, infinite, nil) & solve the matrix with the input vector b
    // Reading from Solution.vector when Solution.type = infinite / nil is undefined behavior
    Solution<T> solve(Vector) const;

    // Get eigenvalues
    //Vector eigenval() const;

    // Get eigenvectors
    //std::vector<Vector> eigenvec() const;
    
    // Diagnoize the matrix (didn't make)
    //matrix<T> diagonize() const;

    // LU Factorization (didn't make)
    //matrix<T> LU() const; 

    // Matrix properties

    // Get rank of matrix
    long rank() const;

    // Get nullity of matrix                                              
    long nullity() const;

    long getRow() const { return this->row; }
    long getCol() const { return this->col; }

    // Get dimension of matrix
    std::vector<long> dim() const;
};
