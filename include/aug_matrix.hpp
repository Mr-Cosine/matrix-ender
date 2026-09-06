#pragma once

#include <vector>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

#include "concepts.hpp"
#include "rational.hpp"
#include "exceptions.hpp"

template <Arithmetic T>
class matrix;

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
         * @note [Header Defn] The method is requested at compile time
         * @note [constexpr] The return value of this method could be resolved at compile time
         * 
         * Instantiates an augmented matrix
         *
         * @param left
         * @param right
         * 
         * @returns augmented_matrix<T>
         */
        constexpr augmented_matrix(matrix<T> inputLeft, matrix<T> inputRight)
            : left(std::move(inputLeft)), right(std::move(inputRight)) {
            if (left.getRow() != right.getRow())
                throw InvalidArgumentException("Matrix have inconsistent row number");
        }
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
         * @note [Header Defn] The method is requested at compile time
         * @note [constexpr] The return value of this method could be resolved at compile time
         * 
         * get matrix on left/right
         *
         * @param side
         * 
         * @returns matrix<T>
         */
        constexpr matrix<T> getSide(Side side) const {
            if (side == Side::LEFT) return left;
            else if (side == Side::RIGHT) return right;
            else throw InvalidArgumentException("Must be either 'left' or 'right'");
        }

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
         * @note [Header Defn] The method is requested at compile time
         * 
         * merge left and right matrix to a giant matrix
         *
         * @param
         * 
         * @returns matrix<T>
         */
        matrix<T> merge() const {
            matrix<T> merged(left.getRow(), left.getCol() + right.getCol());

            for (int r = 0; r < left.getRow(); ++r) {
                for (int c = 0; c < left.getCol(); ++c)
                    merged.put(r, c, left.get(r, c));
                for (int c = 0; c < right.getCol(); ++c)
                    merged.put(r, left.getCol() + c, right.get(r, c));
            }

            return merged;
        }

        /**
         * @bug
         * split a matrix based off the row and col count of this augmented matrix
         *
         * @param inputMatrix
         * 
         * @returns augmented_matrix<T>
         */
        augmented_matrix<T> split(const matrix<T>& inputMatrix) const {
            if (inputMatrix.getRow() != left.getRow() || inputMatrix.getCol() != left.getCol() + right.getCol()) throw ComputationFailedException("Split failed: incompatible size");

            matrix<T> newLeft(this->left.getRow(), this->left.getCol());
            matrix<T> newRight(this->right.getRow(), this->right.getCol());

            for (int r = 0; r < this->left.getRow(); r++) {
                for (int c = 0; c < inputMatrix.getCol(); c++) {
                    if (c < this->left.getCol())
                        newLeft.put(r, c, inputMatrix.get(r, c));
                    else
                        newRight.put(r, c - this->left.getCol(), inputMatrix.get(r, c));
                }
            }

            return augmented_matrix<T>(newLeft, newRight);
        }

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
         * @note [Header Defn] The method is requested at compile time
         * 
         * get reduced echelon form (does not alter this matrix)
         *
         * @param
         * 
         * @returns augmented_matrix<T>
         */
        augmented_matrix<T> rref() const {
            matrix<T> merged(this->merge());
            return this->split(merged.rref(left.getCol()));
        }
                
        /**
         * @bug
         * determine if a vector b is within the span(in column space/ being the linear combination) of matrix
         *
         * @param
         * 
         * @returns augmented_matrix<T>
         */
        bool inspan() const ; 
        
        /**
         * @note [Header Defn] The method is requested at compile time
         * 
         * determine type of solution(unique, infinite, none) & solve the matrix with the input vector b
         *
         * @param
         * 
         * @returns augmented_matrix<T>
         */  
        std::vector<std::vector<T>> solve() const {
            if (right.getCol() > 1) throw MalformedMatrixException("Right hand side have to be 1.");

            augmented_matrix<T> reduced = this->rref();

            struct Pivot {
                int row, col;
                Pivot(int r, int c) : row(r), col(c) {}
            };

            std::vector<Pivot> pivots;
            std::vector<int> zeroRows;

            for (int r = 0; r < reduced.left.getRow(); r++) {
                bool hasNonZero = false;
                for (int c = 0; c < reduced.left.getCol(); ++c) {
                    if (reduced.left.get(r, c) != T(0)) {
                        hasNonZero = true;

                        bool existing = false;
                        for (const auto& pivot : pivots) {
                            if (pivot.col == c) { existing = true; break; }
                        }

                        if (!existing) {
                            pivots.push_back(Pivot(r, c));
                        }
                    }
                }
                if (!hasNonZero) {
                    zeroRows.push_back(r);
                }
            }


            for (int r : zeroRows) {
                if (reduced.right.get(r,0) != T(0)) { throw ComputationFailedException("System has no solution (inconsistent)."); }
            }

            std::vector<std::vector<T>> solution;
            std::vector<T> particular(reduced.left.getCol(), T(0));
            for (const auto& p : pivots) {
                particular[p.col] = reduced.right.get(p.row, 0);
            }

            solution.push_back(particular);

            if (pivots.size() < static_cast<size_t>(left.getCol())) {
                for (int c = 0; c < reduced.left.getCol(); c++) {
                    bool isFree = true; for (const auto& p : pivots) if (p.col == c) { isFree = false; break; }

                    std::vector<T> vec(reduced.left.getCol(), T(0));
                    if (isFree) {
                        vec[c] = T(1);
                        for (const auto& pivot : pivots)
                            vec[pivot.col] = -reduced.left.get(pivot.row, c);
                    }
                    solution.push_back(vec);
                }
            }

            return solution;
        }         
};
