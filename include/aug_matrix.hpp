#pragma once

#include <vector>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

#include "concepts.hpp"
#include "matrix.hpp"

/* Suggestions --> */ /* are wrapped in multiline comments */

template <Arithmetic T>
class augmented_matrix {
    protected:
        matrix<T> left;
        matrix<T> right;

    public:
        enum class Side {
            LEFT,
            RIGHT
        };

        // Added zero constructor for compatibility
        augmented_matrix(): left(), right() {};

        /**
         * @bug
         * Instantiates an augmented matrix
         *
         * @param left
         * @param right
         * 
         * @returns augmented_matrix<T>
         */
        augmented_matrix(matrix<T>, matrix<T>);
        //Construct from a matrix and a group of vector

        
        /**
         * @bug
         * insert value into [r,c] for matrix on left/right (NOTICE: 0 based)
         *
         * @param r
         * @param c
         * @param side
         * 
         * @returns void
         */
        void put(int, int, T, Side);
        
        /**
         * @bug
         * get value from [r,c] for matrix on left/right (NOTICE: 0 based)
         *
         * @param r
         * @param c
         * @param side
         * 
         * @returns T
         */
        T get(int, int, Side) const;

        /**
         * @bug
         * get matrix on left/right
         *
         * @param side
         * 
         * @returns matrix<T>
         */
        matrix<T> getSide(Side) const;

        /**
         * @bug
         * display augmented matrix
         *
         * @param
         * 
         * @returns void
         */
        void display() const;

        /**
         * @bug
         * merge left and right matrix to a giant matrix
         *
         * @param
         * 
         * @returns matrix<T>
         */
        matrix<T> merge() const;

        /**
         * @bug
         * split a matrix based off the row and col count of this augmented matrix
         *
         * @param inputMatrix
         * 
         * @returns augmented_matrix<T>
         */
        augmented_matrix<T> split(const matrix<T>&) const;

        /**
         * @bug
         * row operation: R1 = c1*R1 + c2*R2 to this matrix
         *
         * @param r1
         * @param c1
         * @param r2
         * @param c2
         * 
         * @returns void
         */
        void rowOp(int, int, int, int);   
        
        /**
         * @bug
         * get echelon form (does not alter this matrix)
         *
         * @param
         * 
         * @returns augmented_matrix<T>
         */
        augmented_matrix<T> echelonf() const;

        /**
         * @bug
         * get reduced echelon form (does not alter this matrix)
         *
         * @param
         * 
         * @returns augmented_matrix<T>
         */
        augmented_matrix<T> rref() const;   
        
        /**
         * @bug
         * determine if a vector b is within the span(in column space/ being the linear combination) of matrix
         *
         * @param
         * 
         * @returns augmented_matrix<T>
         */
        bool inSpan() const ; 
        
        /**
         * @bug
         * determine type of solution(unique, infinite, none) & solve the matrix with the input vector b
         *
         * @param
         * 
         * @returns augmented_matrix<T>
         */
        std::vector<std::vector<T>> solve() const;            
};
