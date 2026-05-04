#ifndef MATRIX_H
#define MATRIX_H

#include <exception>
#include <concepts>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <type_traits>

#include "util.hpp"
#include "matrix.hpp"
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
    protected:
        int rowNum;
        int colNum;
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
        // Default constructor (std::variant compatibility)
        matrix() : row(0), col(0) {};

        // Filler constructor
        matrix(long, long, T, FillType = FillType::EVERY);

        // String constructor
        matrix(std::string descriptor, char row_delimiter = ';', char column_delimiter = ',')  {
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

//==============================================================================================================================================
        matrix(int r, int c) : rowNum(r), colNum(c) {}
        matrix(int r, int c, std::string type);
        matrix(const std::vector<std::vector<T>> inputVector);      //Constructor from a 2D vector
        matrix(const matrix<T>& inputMatrix);                       //Copy Constructor                              

        void put(int r, int c, T value);                            //insert value into [r,c]---NOTICE: r and c starts at 0, NOT 1!!!!!!!!!
        T get(int r, int c) const;                                  //get value at [r,c]--------NOTICE: r and c starts at 0, NOT 1!!!!!!!!!
        int getRow() {return rowNum};                               //get row number 
        int getCol() {return colNum};                               //get column number
        void display() const ;                                      //display the matrix
        std::string toString(int loc, std::string dir) const ;      //get row/column as a string
    
        matrix<T> operator+(const matrix<T>& other) const;          //addition
        matrix<T> operator-(const matrix<T>& other) const;          //subtraction
        matrix<T> operator*(const matrix<T>& other) const;          //matrix multiplication
        matrix<T> operator*(T scalar) const;                        //scalar multiplication
        matrix<T>& operator+=(const matrix<T>& other);              //addition assignment
        matrix<T>& operator-=(const matrix<T>& other);              //subtraction assignment
        matrix<T>& operator*=(T scalar);                            //multiplication assignment
        matrix<T> operator-() const;                                //convert to negative

        void rowOp(int r1, int c1, int r2, int c2) ;                //row operation: R1 = c1*R1 + c2*R2
        void rowSwap(int r1, int r2);                               //swap two rows
        matrix<T> echelonf() const;                                 //get echelon form
        matrix<T> rref() const;                                     //get reduceed echelon form
        matrix<T> echelonf(int termination) const;                  //get echelon form from most left to column according to termination
        matrix<T> rref(int termination) const;                      //get reduceed echelon form from most left to column according to termination
        T determinant() const;                                      //get determinant
        matrix<T> transpose() const;                                //get transpose of matrix
        matrix<T> inverse() const;                                  //get matrix inverse
        std::vector<std::complex<double>> eigenval() const;         //get eigen values
        matrix<std::complex<double>> matrix<T>::eigenvec() const;   //get eigen vectors
        bool inSpan(std::vector<T> b) const ;                       //determine if a vector b is within the span(in column space/ being the linear combination) of matrix
        std::vector<std::vector<T>> solve(std::vector<T> b) const;  //determine type of solution(unique, infinite, none) & solve the matrix with the input vector b

        int rank() const;                                           //get rank of matrix
        int nullity() const;                                        //get nullity of matrix
};

#include "matrix.tpp"

#endif