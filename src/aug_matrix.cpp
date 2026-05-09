/*
TODO list:
    1. Compile and debug
    2. After finishing dynamic typing, update logics here
    3. Check for invalid access to data member.
*/

#include "aug_matrix.hpp"

// Use Arithmetic concept instead of typename
template <Arithmetic T>
augmented_matrix<T>::augmented_matrix(matrix<T> inputLeft, matrix<T> inputRight) :
    left(std::move(inputLeft)),
    right(std::move(inputRight))
{
    if (left.getRow() != right.getRow()) throw InvalidArgumentException("Matrix have inconsistent row number");
}

template <Arithmetic T>
void augmented_matrix<T>::put(int r, int c, T value, Side side) {
    if (side == Side::LEFT) {
        if (r >= left.getRow()) throw IndexOutOfBoundException("Row index " + std::to_string(r) + " out of bounds");
        if (c >= left.getCol()) throw IndexOutOfBoundException("Column index " + std::to_string(c) + " out of bounds");

        left.put(r, c, value);
    }
    else if (side == Side::RIGHT) {
        if (r >= right.getRow()) throw IndexOutOfBoundException("Row index " + std::to_string(r) + " out of bounds");
        if (c >= right.getCol()) throw IndexOutOfBoundException("Column index " + std::to_string(c) + " out of bounds");

        right.put(r, c, value);
    }
    else throw InvalidArgumentException("Must be either 'left' or 'right'");
}

template <Arithmetic T>
T augmented_matrix<T>::get(int r, int c, Side side) const{
    if (side == Side::LEFT) {
        if (r >= left.getRow()) throw IndexOutOfBoundException("Row index " + std::to_string(r) + " out of bounds");
        if (c >= left.getCol()) throw IndexOutOfBoundException("Column index " + std::to_string(c) + " out of bounds");

        return left.get(r, c);
    }
    else if (side == Side::RIGHT) {
        if (r >= right.getRow()) throw IndexOutOfBoundException("Row index " + std::to_string(r) + " out of bounds");
        if (c >= right.getCol()) throw IndexOutOfBoundException("Column index " + std::to_string(c) + " out of bounds");

        return right.get(r, c);
    }
    else throw InvalidArgumentException("Must be either 'left' or 'right'");
}

template <Arithmetic T>
matrix<T> augmented_matrix<T>::getSide(Side side) const {
    if (side == Side::LEFT) return left;
    else if (side == Side::RIGHT) return right;
    else throw InvalidArgumentException("Must be either 'left' or 'right'");
}

template <Arithmetic T>
void augmented_matrix<T>::display() const {
    if (left.getRow() == 0) { std::cout << "[empty]"; return; }

    for (int r = 0; r < left.getRow(); r++) {
        std::cout << "[";
        for (int c = 0; c < left.getCol(); c++) {
            std::cout << std::setw(8) << left.get(r, c);
        }
        std::cout << std::setw(8) << "|" << std::endl;
        for (int c = 0; c < right.getCol(); c++) {
            std::cout << std::setw(8) << right.get(r, c);
        }
        std::cout << std::setw(8) << "]" << std::endl;
    }
}

template <Arithmetic T>
matrix<T> augmented_matrix<T>::merge() const {
    matrix<T> merged(left.getRow(), left.getCol() + right.getCol());

    for (int r = 0; r < left.getRow(); ++r) {
        for (int c = 0; c < left.getCol(); ++c)
            merged.put(r, c, left.get(r, c));
        for (int c = 0; c < right.getCol(); ++c)
            merged.put(r, left.getCol() + c, right.get(r, c));
    }

    return merged;
}

template <Arithmetic T>
augmented_matrix<T> augmented_matrix<T>::split(const matrix<T>& inputMatrix) const {
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

template <Arithmetic T>
void augmented_matrix<T>::rowOp(int r1, int c1, int r2, int c2) {
    if (r1 < 0 || r1 >= left.getRow() || r2 < 0 || r2 >= left.getRow()) throw IndexOutOfBoundException("Row index out of bounds");

    if (c1 == 0) throw InvalidArgumentException("cannot replace the row");

    left.ro(r1, c1, r2, c2);
    right.ro(r1, c1, r2, c2);
}

template <Arithmetic T>
augmented_matrix<T> augmented_matrix<T>::echelonf() const {
    matrix<T> merged(this->merge());
    return this->split(merged.ref(left.getCol()));
}

template <Arithmetic T>
augmented_matrix<T> augmented_matrix<T>::rref() const {
    matrix<T> merged(this->merge());
    return this->split(merged.rref(left.getCol()));
}

template <Arithmetic T>
std::vector<std::vector<T>> augmented_matrix<T>::solve() const {
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

    if (pivots.size() < left.getCol()) {
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

template <Arithmetic T>
bool augmented_matrix<T>::inSpan() const {
    try {
        this->solve();
    }
    catch (const InvalidArgumentException&) { return false; }
    return true;
}

template class augmented_matrix<int>;
template class augmented_matrix<double>;
template class augmented_matrix<rational>;