/*
TODO list:
    1. Implement dynamic typing used in calculation
    2. Compile and debug
*/

#include "matrix.hpp"
#include "aug_matrix.hpp"
#include <iostream>

/*
#include <type_traits>
#include <complex>
#include <Eigen/Eigenvalues>
*/

/*
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const matrix<T>& mat) {
    return os << mat.toString();
}
*/

/* Use the conditional compilation block for local testing */
#ifdef DEBUG_MATRIX
using std::cout, std::cin, std::endl;
// Print helper
template <typename... Args>
void print(Args... args) {
    ((cout << args << " "), ...);
    cout << endl;
}
inline std::ostream& operator<<(std::ostream& os, const rational& rational) {
    return os << rational.toString();
}
#endif

template <Arithmetic T>
matrix<T>::matrix(long row, long column, T filler, FillType fill_type)
    : row(row), col(column), data(row, std::vector<T>(column, T(0))) {
        switch (fill_type) {
            case FillType::EVERY:
                this->data = std::vector<std::vector<T>>(row, std::vector<T>(column, filler));
                break;

            case FillType::UPPER_TRI:
                for (int i = 0; i < column; i++) {
                    for (int j = 0; j <= i; j++) {
                        this->data[j][i] = filler;
                    }
                }
                break;
            
            case FillType::UPPER_TRI_R:
                for (int i = 0; i < column; i++) {
                    for (int j = 0; j < row - i; j++) {
                        this->data[j][i] = filler;
                    }
                }
                break;

            case FillType::LOWER_TRI:
                for (int i = 0; i < column; i++) {
                    for (int j = 0; j < row - i; j++) {
                        this->data[row - 1 - j][i] = filler;
                    }
                }
                break;
            
            case FillType::LOWER_TRI_R:
                for (int i = 0; i < column; i++) {
                    for (int j = 0; j <= i; j++) {
                        this->data[row - 1 - j][i] = filler;
                    }
                }
                break;
            
            case FillType::DIAGONAL:
                if (row != col) throw MalformedMatrixException("Non-square diagonal matrix detected");
                for (int i = 0; i < col; i++) {
                    this->data[i][i] = filler;
                }
                break;

            default:
                throw InvalidFillTypeException("fill_type does not conform to any FillType enum");
        }
}

template <Arithmetic T>
void matrix<T>::print(char row_delimiter, char column_delimiter, bool tab, bool pad) const {
    std::cout << (pad ? "\n[" : "[");
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            if ( i != 0 && j == 0) std::cout << " ";
            std::cout << this->data[i][j];
            if (j != this->col - 1) std::cout << column_delimiter;
            if (tab && !(i == this->row - 1 && j == this->col - 1)) std::cout << "\t";
        }
        if (i != this->row - 1) std::cout << row_delimiter;
    }
    std::cout << (pad ? "]\n" : "]") << std::endl;
}

/*
Definition moved to header for compatibility

template <Arithmetic T>
Matrix<T>::Matrix(std::string descriptor, char row_delimiter, char column_delimiter) {
    std::istringstream iss(descriptor);
    std::string row_tkn;
    while (std::getline(iss, row_tkn, row_delimiter) && !row_tkn.empty()) {
        std::vector<T> row;
        std::istringstream row_iss(row_tkn);
        std::string tkn;
        while (std::getline(row_iss, tkn, column_delimiter) && !tkn.empty()) {
            row.push_back(from_string<T>(tkn));
        }
        this->data.push_back(row);
    }
    this->row = this->data.size();
    this->col = this->row > 0 ? this->data[0].size() : 0;
}
*/

#ifdef DEBUG_MATRIX
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const matrix<T>& mat) {
    return os << mat.toString();
}
int main() {
    
    print("=== MATRIX MODULE DEBUG ===\n");

    matrix<int> mymat(4, 4, 1, FillType::UPPER_TRI_R);
    mymat.print();
    matrix<rational> myratmat(4, 4, 2, FillType::LOWER_TRI);
    myratmat.print();
    matrix<int> strmat("1,2,3;4,5,6;7,8,9,;");
    strmat.print();
    cout << "printing matrix:\n" << strmat << endl;

    return 0;
}
#endif

template <Arithmetic T>
void matrix<T>::put(long r, long c, T value) {
    if (r >= this->row) throw IndexOutOfBoundException("Row index " + std::to_string(r) + " out of bounds");
    if (c >= this->col) throw IndexOutOfBoundException("Column index " + std::to_string(c) + " out of bounds");

    this->data[r][c] = value;
    return;
}

template <Arithmetic T>
T matrix<T>::get(long r, long c) const {
    if (r >= this->row) throw IndexOutOfBoundException("Row index " + std::to_string(r) + " out of bounds");
    if (c >= this->col) throw IndexOutOfBoundException("Column index " + std::to_string(c) + " out of bounds");
    
    return this->data[r][c];
}

/*
template <Arithmetic T>
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
*/

/*
template <Arithmetic T>
std::string matrix<T>::toString(int loc, std::string dir) const {
    if (data.empty() || data[0].empty()) { throw std::runtime_error("Matrix is empty"); }

    std::ostringstream output;

    if (dir == "row") {
        if (loc > this->row) throw std::out_of_range("Row index " + std::to_string(loc) + " out of bounds");

        output << "[";
        for (int c = 0; c < this->col; c++) {
            output << std::setw(8) << data[loc - 1][c];
        }
        output << std::setw(8) << "]";

        return output.str();
    }
    else if (dir == "column") {
        if (loc > this->col) throw std::out_of_range("Column index " + std::to_string(loc) + " out of bounds");
        
        output << "[";
        for (int r = 0; r < this->row; r++) {
            output << std::setw(8) << data[r][loc - 1];
        }
        output << std::setw(8) << "]";

        return output.str();
    }
    else throw std::invalid_argument("Direction must be 'row' or 'column', got: " + dir);
}
*/

template <Arithmetic T>
matrix<T> matrix<T>::operator+(const matrix<T>& other) const {
    if (this->row != other.getRow() || this->col != other.getCol()) {
        throw InvalidMatricesPairException("Unmatched matrix size");
    }
    
    matrix<T> result(this->row, this->col);
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            result.put(i, j, this->data[i][j] + other.get(i, j));
        }
    }
    return result;
}

template <Arithmetic T>
matrix<T> matrix<T>::operator-(const matrix<T>& other) const {
    if (this->row != other.getRow() || this->col != other.getCol()) {
        throw InvalidMatricesPairException("Unmatched matrix size");
    }
    
    matrix<T> result(this->row, this->col);
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            result.put(i, j, this->data[i][j] - other.get(i, j));
        }
    }
    return result;
}

template <Arithmetic T>
matrix<T> matrix<T>::operator*(const matrix<T>& other) const {
    if (this->col != other.getRow()) {
        throw InvalidMatricesPairException("Matrix multiplication: columns must match rows");
    }
    
    matrix<T> result(this->row, other.getCol());
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < other.getCol(); j++) {
            T sum = T();
            for (int k = 0; k < this->col; k++) {
                sum += this->data[i][k] * other.get(k, j);
            }
            result.put(i, j, sum);
        }
    }
    return result;
}

template <Arithmetic T>
matrix<T> matrix<T>::operator*(const T scalar) const {
    matrix<T> result(this->row, this->col);
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            result.put(i, j, this->data[i][j] * scalar);
        }
    }
    return result;
}

template <Arithmetic U>
matrix<U> operator*(U scalar, const matrix<U>& m) {
    return m * scalar;
}

template <Arithmetic T>
matrix<T>& matrix<T>::operator+=(const matrix<T>& other) {
    if (this->row != other.getRow() || this->col != other.getCol()) {
        throw InvalidMatricesPairException("Unmatched matrix size");
    }
    
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            this->data[i][j] += other.get(i, j);
        }
    }
    return *this;
}

template <Arithmetic T>
matrix<T>& matrix<T>::operator-=(const matrix<T>& other) {
    if (this->row != other.getRow() || this->col != other.getCol()) {
        throw InvalidMatricesPairException("Unmatched matrix size");
    }
    
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            this->data[i][j] -= other.get(i, j);
        }
    }
    return *this;
}

template <Arithmetic T>
matrix<T>& matrix<T>::operator*=(T scalar) {
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            this->data[i][j] *= scalar;
        }
    }
    return *this;
}

template <Arithmetic T>
matrix<T> matrix<T>::add(const matrix<T>& inputMatrix) const { return this + inputMatrix; }

template <Arithmetic T>
matrix<T> matrix<T>::sub(const matrix<T>& inputMatrix) const { return this - inputMatrix; }

template <Arithmetic T>
matrix<T> matrix<T>::mult(const matrix<T>& inputMatrix) const { return this * inputMatrix; }

template <Arithmetic T>
matrix<T> matrix<T>::div(const matrix<T>& inputMatrix) const { return this * inputMatrix.inverse(); }

template <Arithmetic T>
void matrix<T>::ro(long r1, long n1, long r2, long n2) {
    if (n1 == 0) throw IndexOutOfBoundException("n1 cannot be 0 for row operation r1 = n1r1 + n2r2");
    if (r1 >= this->row || r2 >= this->row ) throw IndexOutOfBoundException("r1 and/or r2 out of bound");
    for (int c = 0; c < this->col; c++) {
        this->data[r1][c] = n1 * this->data[r1][c] + n2 * this->data[r2][c];
    }
}

template <Arithmetic T>
void matrix<T>::co(long c1, long n1, long c2, long n2) {
    if (n1 == 0) throw IndexOutOfBoundException("n1 cannot be 0 for column operation c1 = n1c1 + n2c2");
    if (c1 >= this->col || c2 >= this->col) throw IndexOutOfBoundException("c1 and/or c2 out of bound");
    for (int r = 0; r < this->row; r++) {
        this->data[r][c1] = n1 * this->data[r][c1] + n2 * this->data[r][c2];
    }
}

template <Arithmetic T>
void matrix<T>::re(long r1, long r2) {
    if (r1 >= this->row || r2 >= this->row) throw IndexOutOfBoundException("r1 and/or r2 out of bound");

    std::swap(this->data[r1], this->data[r2]);
}

template <Arithmetic T>
void matrix<T>::ce(long c1, long c2) {
    if (c1 >= this->col || c2 >= this->col) IndexOutOfBoundException("c1 and/or c2 out of bound");

    std::swap(this->data[c1], this->data[c2]);
}

template <Arithmetic T>
matrix<T> matrix<T>::ref(long termination) const {
    if (this->data.empty() || this->data[0].empty()) throw MalformedMatrixException("Matrix is empty");

    matrix<T> temp(*this);
    int lead = 0;
    int r = 0;

     while (r < this->row && lead < termination) {
        int i = r;
        while (i < this->row && temp.get(i, lead) == T())
            i++;

        if (i < this->row) {
            if (i != r)
                temp.re(r, i);

            T pivot = temp.get(r, lead);

            for (int j = r + 1; j < this->row; ++j) {
                T factor = temp.get(j, lead) / pivot;
                for (int k = lead; k < this->col; ++k)
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

template <Arithmetic T>
matrix<T> matrix<T>::ref() const { return ref(this->col); }

template <Arithmetic T>
matrix<T> matrix<T>::rref(long termination) const {
    if (this->data.empty() || this->data[0].empty())
        throw MalformedMatrixException("Matrix is empty");

    matrix<T> temp = this->ref(termination);

    for (int r = this->row - 1; r >= 0; --r) {
        int pivotCol = -1;
        for (int c = 0; c < termination; ++c) {
            if (temp.get(r, c) != T()) {
                pivotCol = c;
                break;
            }
        }

        if (pivotCol == -1) continue;

        T pivot = temp.get(r, pivotCol);
        for (int c = pivotCol; c < this->col; ++c)
            temp.put(r, c, temp.get(r, c)/pivot);

        for (int i = r - 1; i >= 0; --i) {
            T factor = temp.get(i, pivotCol);
            if (factor != T()) {
                for (int c = pivotCol; c < this->col; ++c)
                    temp.put(i, c, temp.get(i, c) - factor*temp.get(r, c));
            }
        }
    }
    return temp;
}

template <Arithmetic T>
matrix<T> matrix<T>::rref() const { return rref(this->col); }


template <Arithmetic T>
bool matrix<T>::inspan(Vector vector) const {
    matrix<T> rightside(vector);
    augmented_matrix<T> augmented(this, rightside);
    Solution<T> sol;

    try { sol.vector_group = augmented.solve(); }
    catch (std::runtime_error) { return false; }

    return true;
}

template <Arithmetic T>
Solution<T> matrix<T>::solve(Vector vector) const {
    matrix<T> rightside(vector);
    augmented_matrix<T> augmented(this, rightside);
    Solution<T> sol;

    try { sol.vector_group = augmented.solve(); }
    catch (std::runtime_error) { sol.type = Solution<T>::SolutionType::NIL; return sol;}

    if (sol.vector_group.size() > 1) {
        sol.type = Solution<T>::SolutionType::INFINITE;
        return sol;
    }

    sol.type = Solution<T>::SolutionType::UNIQUE;
    return sol;
}

template <Arithmetic T>
T matrix<T>::det() const {
    if (this->row != this->col) throw MalformedMatrixException("Non-square matrix");

    if (this->row == 1) return data[0][0];
    if (this->row == 2) return data[0][0] * data[1][1] - data[0][1] * data[1][0];

    T det = 0;
    for (int c = 0; c < this->col; c++) {
        matrix<T> subMatrix(this->row - 1, this->col - 1);

        for (int i = 1; i < this->row; i++) {
            for (int j = 0; j < this->col; j++) {
                if (j == c) continue;
                int subCol = (j < c) ? j : j - 1;
                subMatrix.put(i - 1, subCol, data[i][j]);
            }
        }
        
        det = (c % 2 != 0)? det - data[0][c] * subMatrix.det(): det + data[0][c] * subMatrix.det();
    }

    return det;
}

template <Arithmetic T>
matrix<T> matrix<T>::transpose() const {
    matrix<T> transpose(this->col, this->row);

    for (int r = 0; r < this->row; r++){
        for (int c = 0; c < this->col; c++) {
            transpose.put(c, r, data[r][c]);
        }
    }

    return transpose;
}

template <Arithmetic T>
matrix<T> matrix<T>::inverse() const {
    if (this->row != this->col || this->det() == 0) throw NotInvertibleMatrixException("Matrix is not invertible");

    matrix<T> I(this->row, this->col, 1, FillType::DIAGONAL);

    augmented_matrix<T> aug(*this, I);
    aug = aug.rref();

    return aug.getSide(augmented_matrix<T>::Side::RIGHT);
}

/*
TODO: Reimplement

template <Arithmetic T>
std::vector<std::complex<double>> matrix<T>::eigenval() const {
    if (this->row != this->col) throw std::invalid_argument("Eigenvalue not defined for non-square matrix");
    
    Eigen::MatrixXd A(this->row, this->col);
    for (int i = 0; i < this->row; i++)
        for (int j = 0; j < this->col; j++)
            A(i, j) = static_cast<double>(data[i][j]);

    Eigen::EigenSolver<Eigen::MatrixXd> solver(A);

    if (solver.info() != Eigen::Success) throw std::runtime_error("Eigenvalue computation failed");

    Eigen::VectorXcd eigenvalues = solver.eigenvalues();
    return std::vector<std::complex<double>>(eigenvalues.data(),
                                            eigenvalues.data() + eigenvalues.size());
}
*/

/*
TODO: reimplement

template <Arithmetic T>
matrix<std::complex<double>> matrix<T>::eigenvec() const {
    if (this->row != this->col)
        throw std::invalid_argument("Eigenvectors not defined for non-square matrix");

    Eigen::MatrixXd A(this->row, this->col);
    for (int i = 0; i < this->row; ++i)
        for (int j = 0; j < this->col; ++j)
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
*/

template <Arithmetic T>
long matrix<T>::rank() const {
    matrix<T> reduced = this->ref();
    int rank = 0;
    for (int i = 0; i < this->row; ++i) {
        for (int j = 0; j < this->col; ++j) {
            if (reduced.get(i, j) != T(0)) { ++rank; break; }
        }
    }
    return rank;
}

template <Arithmetic T>
long matrix<T>::nullity() const { return this->col - this->rank(); }

template <Arithmetic T>
std::vector<long> matrix<T>::dim() const {
    return std::vector<long>(row, col);
}