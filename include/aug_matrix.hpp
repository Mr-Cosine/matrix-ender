#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "concepts.hpp"

/* Suggestions --> */ /* are wrapped in multiline comments */

// Forward declare matrix to prevent circular import
template <Arithmetic T>
class matrix;

template <Arithmetic T>
class augmented_matrix {
    protected:
        matrix<T> left;
        matrix<T> right;

        // Custom exception type here
        // class MyException: public std::exception;

        // Custom enum type here (dont use C-style enums)
        enum class Side {
            LEFT,
            RIGHT
        };

    public:
        /* Use function documentations, example below */
        /**
         * @bug Here's a bug
         * Instantiates an augmented matrix
         *
         * @param left 
         * @param right
         * 
         * @returns augmented_matrix<T>
         */
        augmented_matrix(matrix<T> left, matrix<T> right);
        //Construct from a matrix and a group of vector

        // Added zero constructor for compatibility
        augmented_matrix(): left(), right() {};

        /* Do not override any default constructors as they are supplied by the compiler 
        augmented_matrix(const augMatrix<T>& inputMatrix);
        //Copy Constructor*/


        /* Don't name the arguments in function declarations */
        void put(int, int, T, std::string);
        //insert value into [r,c]---NOTICE: r and c starts at 0, NOT 1!!!!!!!!!   
        
        
        T get(int r, int c, std::string side) const;                    //get value at [r,c]--------NOTICE: r and c starts at 0, NOT 1!!!!!!!!!
        matrix<T> getLeft() const;                                      //get left side
        matrix<T> getRight() const;                                     //get right side
        void display() const ;                                          //display the matrix
        matrix<T> merge() const;                                        //merge the left and right together
        augmented_matrix<T> split(const matrix<T>& inputMatrix) const;                //split a matrix based off the row and col count of this augmented matrix

        void rowOp(int r1, int c1, int r2, int c2);                     //row operation: R1 = c1*R1 + c2*R2
        augmented_matrix<T> echelonf() const;                                  //get echelon form
        augmented_matrix<T> rref() const;                                      //get reduceed echelon form
        bool inSpan() const ;                                           //determine if a vector b is within the span(in column space/ being the linear combination) of matrix
        std::vector<std::vector<T>> solve() const;                      //determine type of solution(unique, infinite, none) & solve the matrix with the input vector b
};
