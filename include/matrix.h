#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

template <typename T>
class matrix {
    protected:
        int rowNum;
        int colNum;
        std::vector<std::vector<T>> data;

    public:
        matrix(int r, int c) : rowNum(r), colNum(c) {}
        matrix(int r, int c, std::string type);
        matrix(const std::vector<std::vector<T>> inputVector);      //Constructor from a 2D vector
        matrix(const matrix<T>& inputMatrix);                       //Copy Constructor                              

        void put(int r, int c, T value);                            //insert value into [r,c]---NOTICE: r and c starts at 0, NOT 1!!!!!!!!!
        T get(int r, int c) const;                                  //get value at [r,c]--------NOTICE: r and c starts at 0, NOT 1!!!!!!!!!
        int getRow();                                               //get row number 
        int getCol();                                               //get column number
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