/*
TODO list:
    1. Implement dynamic typing used in calculation
    2. Compile and debug
*/

#include "matrix.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <complex>
#include <type_traits>
#include <Eigen/Eigenvalues>

template <typename T>
class augMatrix;

template <typename T>
matrix<T>::matrix(int r, int c, std::string type) :
    rowNum(r), 
    colNum(c),
    data(r, std::vector<T>(c, T()))
{
    if (type == "identity") {
        if (r != c) throw std::invalid_argument("Identity matrix have to be square");
        for (int i = 0; i < rowNum; i++) {
            this->put(i, i, 1);
        }
    }
    else if (type == "zero") {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) this->put(i, j, T(0));
        }
    }
    else if (type == "one") {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) this->put(i, j, T(1));
        }
    }
    else if (type == "upper_triangle") {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if (j >= i) this->put(i, j, T(1));
                else this->put(i, j, T(0));
            }
        }
    }
    else if (type == "lower_triangle") {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if (j <= i) this->put(i, j, T(1));
                else this->put(i, j, T(0));
            }
        }
    }
    else throw std::invalid_argument("Unknown matrix type, got: " + type);
}

template <typename T>
matrix<T>::matrix(const matrix<T>& inputMatrix) : 
    rowNum(inputMatrix.getRow()), 
    colNum(inputMatrix.getCol()),
    data(rowNum, std::vector<T>(colNum))
{
    for (int r = 0; r < rowNum; r++) {
        for (int c = 0; c < colNum; c++) {
            data[r][c] = inputMatrix.get(r, c);
        }
    }
}

template <typename T>
matrix<T>::matrix(const std::vector<std::vector<T>> inputVector) : 
    rowNum(static_cast<int>(inputVector.size())),
    colNum(rowNum > 0 ? static_cast<int>(inputVector[0].size()) : 0),
    data(std::move(inputVector))
{
    for (const auto& row : inputVector) {
        if (static_cast<int>(row.size()) != colNum) {
            throw std::invalid_argument(
                "All rows must have the same number of columns"
            );
        }
    }
}

template <typename T>
void matrix<T>::put(int r, int c, T value) {
    if (r >= rowNum) throw std::out_of_range("Row index " + std::to_string(r) + " out of bounds");
    if (c >= colNum) throw std::out_of_range("Column index " + std::to_string(c) + " out of bounds");

    data[r][c] = value;
    return;
}

template <typename T>
T matrix<T>::get(int r, int c = 0) const {
    if (r >= rowNum) throw std::out_of_range("Row index " + std::to_string(r) + " out of bounds");
    if (c >= colNum) throw std::out_of_range("Column index " + std::to_string(c) + " out of bounds");
    
    return data[r][c];
}

template <typename T>
void matrix<T>::display() const {
    if (data.empty() || data[0].empty()) { throw std::runtime_error("Matrix is empty"); }

    for (const auto& row : data) {
        std::cout << '[';
        for (const auto& entry : row) {
            std::cout << std::setw(8) << entry;
        }
        std::cout << std::setw(8) << ']\n';
    }

    return;
};

template<typename T>
std::string matrix<T>::toString(int loc, std::string dir) const {
    if (data.empty() || data[0].empty()) { throw std::runtime_error("Matrix is empty"); }

    std::ostringstream output;

    if (dir == "row") {
        if (loc > rowNum) throw std::out_of_range("Row index " + std::to_string(loc) + " out of bounds");

        output << "[";
        for (int c = 0; c < colNum; c++) {
            output << std::setw(8) << data[loc - 1][c];
        }
        output << std::setw(8) << "]";

        return output.str();
    }
    else if (dir == "column") {
        if (loc > colNum) throw std::out_of_range("Column index " + std::to_string(loc) + " out of bounds");
        
        output << "[";
        for (int r = 0; r < rowNum; r++) {
            output << std::setw(8) << data[r][loc - 1];
        }
        output << std::setw(8) << "]";

        return output.str();
    }
    else throw std::invalid_argument("Direction must be 'row' or 'column', got: " + dir);
}

template <typename T>
matrix<T> matrix<T>::operator+(const matrix<T>& other) const {
    if (rowNum != other.getRow() || colNum != other.getCol()) {
        throw std::invalid_argument("Unmatched matrix size");
    }
    
    matrix<T> result(rowNum, colNum);
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            result.put(i, j, data[i][j] + other.get(i, j));
        }
    }
    return result;
}

template <typename T>
matrix<T> matrix<T>::operator-(const matrix<T>& other) const {
    if (rowNum != other.getRow() || colNum != other.getCol()) {
        throw std::invalid_argument("Unmatched matrix size");
    }
    
    matrix<T> result(rowNum, colNum);
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            result.put(i, j, data[i][j] - other.get(i, j));
        }
    }
    return result;
}

template <typename T>
matrix<T> matrix<T>::operator*(const matrix<T>& other) const {
    if (colNum != other.getRow()) {
        throw std::invalid_argument("Matrix multiplication: columns must match rows");
    }
    
    matrix<T> result(rowNum, other.getCol());
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < other.getCol(); j++) {
            T sum = T();
            for (int k = 0; k < colNum; k++) {
                sum += data[i][k] * other.get(k, j);
            }
            result.put(i, j, sum);
        }
    }
    return result;
}

template <typename T>
matrix<T> matrix<T>::operator*(const T scalar) const {
    matrix<T> result(rowNum, colNum);
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            result.put(i, j, data[i][j] * scalar);
        }
    }
    return result;
}

template <typename U>
matrix<U> operator*(U scalar, const matrix<U>& m) {
    return m * scalar;
}

template <typename T>
matrix<T>& matrix<T>::operator+=(const matrix<T>& other) {
    if (rowNum != other.getRow() || colNum != other.getCol()) {
        throw std::invalid_argument("Unmatched matrix size");
    }
    
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            data[i][j] += other.get(i, j);
        }
    }
    return *this;
}

template <typename T>
matrix<T>& matrix<T>::operator-=(const matrix<T>& other) {
    if (rowNum != other.getRow() || colNum != other.getCol()) {
        throw std::invalid_argument("Unmatched matrix size");
    }
    
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            data[i][j] -= other.get(i, j);
        }
    }
    return *this;
}

template <typename T>
matrix<T>& matrix<T>::operator*=(T scalar) {
    for (int i = 0; i < rowNum; i++) {
        for (int j = 0; j < colNum; j++) {
            data[i][j] *= scalar;
        }
    }
    return *this;
}

template <typename T>
void matrix<T>::rowOp(int r1, int c1, int r2, int c2) {
    if (r1 >= rowNum || r2 >= rowNum || c1 == 0) throw std::invalid_argument("Invalid argument for r1, r2, and c1");

    for (int c = 0; c < colNum; c++) {
        data[r1][c] = c1 * data[r1][c] + c2 * data[r2][c];
    }

    return;
}

template <typename T>
void matrix<T>::rowSwap(int r1, int r2) {
    if (r1 >= rowNum || r2 >= rowNum) throw std::invalid_argument("r1 and/or r2 out of bound");

    std::swap(data[r1], data[r2]);
}

template <typename T>
matrix<T> matrix<T>::echelonf(int termination) const {
    if (data.empty() || data[0].empty()) throw std::runtime_error("Matrix is empty");

    matrix<T> temp(*this);
    int lead = 0;
    int r = 0;

     while (r < rowNum && lead < termination) {
        int i = r;
        while (i < rowNum && temp.get(i, lead) == T())
            i++;

        if (i < rowNum) {
            if (i != r)
                temp.rowSwap(r, i);

            T pivot = temp.get(r, lead);

            for (int j = r + 1; j < rowNum; ++j) {
                T factor = temp.get(j, lead) / pivot;
                for (int k = lead; k < colNum; ++k)
                    temp.put(j, k, temp.get(j, k) - factor * temp.get(r, k));
            }

            r++;
            lead++;
        } else {
            lead++;
        }
    }

    return temp;
}

template <typename T>
matrix<T> matrix<T>::echelonf() const { return echelonf(colNum); }

template <typename T>
matrix<T> matrix<T>::rref(int termination) const {
    if (data.empty() || data[0].empty())
        throw std::runtime_error("Matrix is empty");

    matrix<T> temp = this->echelonf(termination);

    for (int r = rowNum - 1; r >= 0; --r) {
        int pivotCol = -1;
        for (int c = 0; c < termination; ++c) {
            if (temp.get(r, c) != T()) {
                pivotCol = c;
                break;
            }
        }

        if (pivotCol == -1) continue;

        T pivot = temp.get(r, pivotCol);
        for (int c = pivotCol; c < colNum; ++c)
            temp.put(r, c, temp.get(r, c)/pivot);

        for (int i = r - 1; i >= 0; --i) {
            T factor = temp.get(i, pivotCol);
            if (factor != T()) {
                for (int c = pivotCol; c < colNum; ++c)
                    temp.put(i, c, temp.get(i, c) - factor*temp.get(r, c));
            }
        }
    }
    return temp;
}

template <typename T>
matrix<T> matrix<T>::rref() const { return rref(colNum); }

template <typename T>
T matrix<T>::determinant() const {
    if (rowNum != colNum) throw std::invalid_argument("Non-square matrix");

    if (rowNum == 1) return data[0][0];
    if (rowNum == 2) return data[0][0] * data[1][1] - data[0][1] * data[1][0];

    T det = 0;
    for (int c = 0; c < colNum; c++) {
        matrix<T> subMatrix(rowNum - 1, colNum - 1);

        for (int i = 1; i < rowNum; i++) {
            for (int j = 0; j < colNum; j++) {
                if (j == c) continue;
                int subCol = (j < c) ? j : j - 1;
                subMatrix.put(i - 1, subCol, data[i][j]);
            }
        }
        
        det = (c % 2 != 0)? det - data[0][c] * subMatrix.determinant(): det + data[0][c] * subMatrix.determinant();
    }

    return det;
}

template <typename T>
matrix<T> matrix<T>::transpose() const {
    matrix<T> transpose(colNum, rowNum);

    for (int r = 0; r < rowNum; r++){
        for (int c = 0; c < colNum; c++) {
            transpose.put(c, r, data[r][c]);
        }
    }

    return transpose;
}

template <typename T>
matrix<T> matrix<T>::inverse() const {
    if (this->rowNum != this->colNum || this->determinant() == 0) throw std::runtime_error("Matrix is not invertible");

    matrix<T> I(this->rowNum, this->colNum, "identity");
    augMatrix<T> aug(*this, I);
    aug = aug.rref();

    return aug.getRight();
}

template <typename T>
std::vector<std::complex<double>> matrix<T>::eigenval() const {
    if (rowNum != colNum) throw std::invalid_argument("Eigenvalue not defined for non-square matrix");
    
    Eigen::MatrixXd A(rowNum, colNum);
    for (int i = 0; i < rowNum; i++)
        for (int j = 0; j < colNum; j++)
            A(i, j) = static_cast<double>(data[i][j]);

    Eigen::EigenSolver<Eigen::MatrixXd> solver(A);

    if (solver.info() != Eigen::Success) throw std::runtime_error("Eigenvalue computation failed");

    Eigen::VectorXcd eigenvalues = solver.eigenvalues();
    return std::vector<std::complex<double>>(eigenvalues.data(),
                                            eigenvalues.data() + eigenvalues.size());
}

template <typename T>
matrix<std::complex<double>> matrix<T>::eigenvec() const {
    if (rowNum != colNum)
        throw std::invalid_argument("Eigenvectors not defined for non-square matrix");

    Eigen::MatrixXd A(rowNum, colNum);
    for (int i = 0; i < rowNum; ++i)
        for (int j = 0; j < colNum; ++j)
            A(i, j) = static_cast<double>(data[i][j]);

    Eigen::EigenSolver<Eigen::MatrixXd> solver(A);
    if (solver.info() != Eigen::Success)
        throw std::runtime_error("Eigen decomposition failed");

    Eigen::MatrixXcd eigenvectors = solver.eigenvectors();
    int n = eigenvectors.rows();
    int m = eigenvectors.cols();

    matrix<std::complex<double>> result(n,m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            result.put(i, j, eigenvectors(i, j));
    return result;
}

template <typename T>
int matrix<T>::rank() const {
    matrix<T> reduced = this->echelonf();
    int rank = 0;
    for (int i = 0; i < rowNum; ++i) {
        for (int j = 0; j < colNum; ++j) {
            if (reduced.get(i, j) != T(0)) { ++rank; break; }
        }
    }
    return rank;
}

template <typename T>
int matrix<T>::nullity() const { return colNum - this->rank(); }