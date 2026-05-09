/*
TODO list:
    1. Compile and debug
    2. After finishing dynamic typing, update logics here
    3. Check for invalid access to data member.
*/

#include "aug_matrix.hpp"

// Use Arithmetic concept instead of typename


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
bool augmented_matrix<T>::inSpan() const {
    try {
        this->solve();
    }
    catch (const InvalidArgumentException&) { return false; }
    return true;
}

/*
template class augmented_matrix<int>;
template class augmented_matrix<double>;
template class augmented_matrix<rational>;
*/