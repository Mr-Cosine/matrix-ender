#include "augMatrix.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

template <typename T>
augMatrix<T>::augMatrix(matrix<T> inputLeft, matrix<T> inputRight) :
    left(std::move(inputLeft)),
    right(std::move(inputRight))
{
    if (left.getRow() != right.getRow()) throw std::invalid_argument("Matrix have inconsistent row number");
}

template <typename T>
augMatrix<T>::augMatrix(const augMatrix<T>& inputMatrix) :    
    left(inputMatrix.left),
    right(inputMatrix.right)
{}

template <typename T>
void augMatrix<T>::put(int r, int c, T value, std::string side) {
    if (side == "left") {
        if (r >= left.getRow()) throw std::out_of_range("Row index " + std::to_string(r) + " out of bounds");
        if (c >= left.getCol()) throw std::out_of_range("Column index " + std::to_string(c) + " out of bounds");
        
        left.put(r, c, value);
    }
    else if (side == "right") {
        if (r >= right.getRow()) throw std::out_of_range("Row index " + std::to_string(r) + " out of bounds");
        if (c >= right.getCol()) throw std::out_of_range("Column index " + std::to_string(c) + " out of bounds");

        right.put(r, c, value);
    }
    else throw std::invalid_argument("Must be either 'left' or 'right', got: " + side);
}

template <typename T>
T augMatrix<T>::get(int r, int c, std::string side) const{
    if (side == "left") {
        if (r >= left.getRow()) throw std::out_of_range("Row index " + std::to_string(r) + " out of bounds");
        if (c >= left.getCol()) throw std::out_of_range("Column index " + std::to_string(c) + " out of bounds");
        
        return left.get(r, c);
    }
    else if (side == "right") {
        if (r >= right.getRow()) throw std::out_of_range("Row index " + std::to_string(r) + " out of bounds");
        if (c >= right.getCol()) throw std::out_of_range("Column index " + std::to_string(c) + " out of bounds");

        return right.get(r, c);
    }
    else throw std::invalid_argument("Must be either 'left' or 'right', got: " + side);
}

template <typename T>
matrix<T> augMatrix<T>::getLeft() const { return left; }

template <typename T>
matrix<T> augMatrix<T>::getRight() const { return right; }

template <typename T>
void augMatrix<T>::display() const {
    if (left.getRow() == 0) { std::cout << "[empty]"; return; }

    for (int i = 0; i < left.getRow(); i++) {

        std::string leftPart  = left.toString(i, "row");
        std::string rightPart = right.toString(i, "row");

        if (!leftPart.empty()) leftPart.pop_back();
        if (!rightPart.empty()) rightPart.erase(0, 1);

        std::cout << leftPart << " | " << rightPart << '\n';
    }
}

template <typename T>
matrix<T> augMatrix<T>::merge() const {
    matrix<T> merged(left.getRow(), left.getCol() + right.getCol());

    for (int r = 0; r < left.getRow(); ++r) {
        for (int c = 0; c < left.getCol(); ++c)
            merged.put(r, c, left.get(r, c));
        for (int c = 0; c < right.getCol(); ++c)
            merged.put(r, left.getCol() + c, right.get(r, c));
    }
    
    return merged;
}

template <typename T>
augMatrix<T> augMatrix<T>::split(const matrix<T>& inputMatrix) const {
    if (inputMatrix.getRow() != left.getRow() || inputMatrix.getCol() != left.getCol() + right.getCol()) throw std::runtime_error("Split failed: incompatible size");
    
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

    return augMatrix<T>(newLeft, newRight);
}

template <typename T>
void augMatrix<T>::rowOp(int r1, int c1, int r2, int c2) {
    if (r1 < 0 || r1 >= left.getRow() || r2 < 0 || r2 >= left.getRow()) throw std::invalid_argument("Row index out of bounds");

    if (c1 == 0) throw std::invalid_argument("cannot replace the row");

    left.rowOp(r1, c1, r2, c2);
    right.rowOp(r1, c1, r2, c2);

    return;
}

template <typename T>
augMatrix<T> augMatrix<T>::echelonf() const {
    matrix<T> merged(this->merge());
    return this->split(merged.echelonf(left.getCol()));
}

template <typename T>
augMatrix<T> augMatrix<T>::rref() const {
    matrix<T> merged(this->merge());
    return this->split(merged.rref(left.getCol()));
}

template <typename T>
std::vector<std::vector<T>> augMatrix<T>::solve() const {
    if (right.getCol() > 1) throw std::runtime_error("Right hand side have to be 1.");
    
    augMatrix<T> reduced = this->rref();

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
        if (reduced.right.get(r) != T(0)) { throw std::runtime_error("System has no solution (inconsistent)."); }
    }

    std::vector<std::vector<T>> solution;
    std::vector<T> particular(reduced.left.getCol(), T(0));
    for (const auto& p : pivots) {
        particular[p.col] = reduced.right.get(p.row);
    }

    solution.push_back(particular);

    for (int c = 0; c < reduced.left.getCol(); c++) {
        bool isFree = true; for (const auto& p : pivots) if (p.col == c) { isFree = false; break; }

        std::vector<T> vec(reduced.left.getCol(), T(0));
        if (isFree) {
            vec[c] = T(1);
            for (const auto& pivot : pivots)
                vec[pivot.col] = -reduced.left.get(p.row, c);
        }
        solution.push_back(vec);
    }

    return solution;
}

template <typename T>
bool augMatrix<T>::inSpan() const {
    try {
        this->solve();
    }
    catch (const std::runtime_error&) { return false; }
    return true;
}