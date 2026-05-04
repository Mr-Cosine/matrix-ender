#ifndef AUGMATRIX_H
#define AUGMATRIX_H

#include "matrix.hpp"
#include <iostream>
#include <string>
#include <vector>

template <typename T>
class augMatrix {
    protected:
        matrix<T> left;
        matrix<T> right;

    public:
        augMatrix(matrix<T> left, matrix<T> right);                     //Construct from a matrix and a group of vector
        augMatrix(const augMatrix<T>& inputMatrix);                     //Copy Constructor      

        void put(int r, int c, T value, std::string side);              //insert value into [r,c]---NOTICE: r and c starts at 0, NOT 1!!!!!!!!!
        T get(int r, int c, std::string side) const;                    //get value at [r,c]--------NOTICE: r and c starts at 0, NOT 1!!!!!!!!!
        matrix<T> getLeft() const;                                      //get left side
        matrix<T> getRight() const;                                     //get right side
        void display() const ;                                          //display the matrix
        matrix<T> merge() const;                                        //merge the left and right together
        augMatrix<T> split(const matrix<T>& inputMatrix) const;                //split a matrix based off the row and col count of this augmented matrix

        void rowOp(int r1, int c1, int r2, int c2);                     //row operation: R1 = c1*R1 + c2*R2
        augMatrix<T> echelonf() const;                                  //get echelon form
        augMatrix<T> rref() const;                                      //get reduceed echelon form
        bool inSpan() const ;                                           //determine if a vector b is within the span(in column space/ being the linear combination) of matrix
        std::vector<std::vector<T>> solve() const;                      //determine type of solution(unique, infinite, none) & solve the matrix with the input vector b
};

#endif