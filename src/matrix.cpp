/*
TODO list:
    1. Implement dynamic typing used in calculation
    2. Compile and debug
*/

#include "matrix.hpp"
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

    matrix<int> a{
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
    (a).print();

    (a * matrix<int>{
        {2,1,1},
        {2,1,1},
        {2,1,1}
    }).print();

    matrix<rational> t("[0,0;7/4,10/3]");
    t.print();
    t.ref().print();

    return 0;
}
#endif

template <Arithmetic T>
matrix<T>::matrix(std::initializer_list<std::initializer_list<T>> list) {
    this->row = list.size();
    this->col = (*list.begin()).size();
    this->data = std::vector<std::vector<T>>(this->row, std::vector<T>(this->col));

    int i = 0;
    for (const std::initializer_list<T>& row: list) {
        int j = 0;
        for (const T& entry: row) {
            this->data[i][j] = entry;
            j++;
        }
        i++;
    }
}

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

template <Arithmetic T>
matrix<T>::matrix(long row, long column)
    : row(row), col(column), data(std::vector<std::vector<T>>(row, std::vector<T>(col, 0))) {};

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

/*
template <Arithmetic T>
matrix<T>::matrix(int r, int c, std::string type) :
    this->row(r), 
    this->col(c),
    data(r, std::vector<T>(c, T()))
{
    if (type == "identity") {
        if (r != c) throw std::invalid_argument("Identity matrix have to be square");
        for (int i = 0; i < this->row; i++) {
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
*/

/*
template <Arithmetic T>
matrix<T>::matrix(const matrix<T>& inputMatrix) : 
    this->row(inputMatrix.getRow()), 
    this->col(inputMatrix.getCol()),
    data(this->row, std::vector<T>(this->col))
{
    for (int r = 0; r < this->row; r++) {
        for (int c = 0; c < this->col; c++) {
            data[r][c] = inputMatrix.get(r, c);
        }
    }
}
*/

/*
template <Arithmetic T>
matrix<T>::matrix(const std::vector<std::vector<T>> inputVector) : 
    this->row(static_cast<int>(inputVector.size())),
    this->col(this->row > 0 ? static_cast<int>(inputVector[0].size()) : 0),
    data(std::move(inputVector))
{
    for (const auto& row : inputVector) {
        if (static_cast<int>(row.size()) != this->col) {
            throw std::invalid_argument(
                "All rows must have the same number of columns"
            );
        }
    }
}
*/

template <Arithmetic T>
void matrix<T>::put(long r, long c, T value) {
    if (r >= this->row) throw std::out_of_range("Row index " + std::to_string(r) + " out of bounds");
    if (c >= this->col) throw std::out_of_range("Column index " + std::to_string(c) + " out of bounds");

    data[r][c] = value;
    return;
}

template <Arithmetic T>
T matrix<T>::get(long r, long c) const {
    if (r >= this->row) throw std::out_of_range("Row index " + std::to_string(r) + " out of bounds");
    if (c >= this->col) throw std::out_of_range("Column index " + std::to_string(c) + " out of bounds");
    
    return data[r][c];
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
    if (this->row != other.row || this->col != other.col) {
        throw std::invalid_argument("operator+: Incompatible matrix shape.");
    }
    
    matrix<T> result(this->row, this->col);
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            result.put(i, j, data[i][j] + other.get(i, j));
        }
    }
    return result;
}

template <Arithmetic T>
matrix<T> matrix<T>::operator-(const matrix<T>& other) const {
    if (this->row != other.getRow() || this->col != other.getCol()) {
        throw std::invalid_argument("operator-: Incompatible shape.");
    }
    
    matrix<T> result(this->row, this->col);
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            result.put(i, j, data[i][j] - other.get(i, j));
        }
    }
    return result;
}

template <Arithmetic T>
matrix<T> matrix<T>::operator*(const matrix<T>& other) const {
    if (this->col != other.row) {
        throw std::invalid_argument("operator*: Incompatible shape.");
    }
    
    matrix<T> result(this->row, other.col);
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < other.col; j++) {
            T sum = T();
            for (int k = 0; k < this->col; k++) {
                sum += data[i][k] * other.get(k, j);
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
            result.put(i, j, data[i][j] * scalar);
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
        throw std::invalid_argument("operator+=: Incompatible matrix shape.");
    }
    
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            data[i][j] += other.get(i, j);
        }
    }
    return *this;
}

template <Arithmetic T>
matrix<T>& matrix<T>::operator-=(const matrix<T>& other) {
    if (this->row != other.getRow() || this->col != other.getCol()) {
        throw std::invalid_argument("operator-: Incompatible matrix shape.");
    }
    
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            data[i][j] -= other.get(i, j);
        }
    }
    return *this;
}

template <Arithmetic T>
matrix<T>& matrix<T>::operator*=(T scalar) {
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            data[i][j] *= scalar;
        }
    }
    return *this;
}

template <Arithmetic T>
void matrix<T>::ro(long r1, long c1, long r2, long c2) {
    if (r1 >= this->row || r2 >= this->row || c1 == 0) throw std::invalid_argument("Invalid argument for r1, r2, and c1");
    for (int c = 0; c < this->col; c++) {
        data[r1][c] = c1 * data[r1][c] + c2 * data[r2][c];
    }
}

template <Arithmetic T>
void matrix<T>::re(long r1, long r2) {
    if (r1 >= this->row || r2 >= this->row) throw std::invalid_argument("r1 and/or r2 out of bound");

    std::swap(data[r1], data[r2]);
}

template <Arithmetic T>
matrix<T> matrix<T>::ref(long stop_at) const {
    if (data.empty() || data[0].empty()) throw std::runtime_error("Matrix is empty");

    matrix<T> temp(*this);
    int lead = 0;
    int r = 0;

     while (r < this->row && lead < stop_at) {
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
matrix<T> matrix<T>::rref(long stop_at) const {
    if (data.empty() || data[0].empty())
        throw std::runtime_error("Matrix is empty");

    matrix<T> temp = this->ref(stop_at);

    for (int r = this->row - 1; r >= 0; --r) {
        int pivotCol = -1;
        for (int c = 0; c < stop_at; ++c) {
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
T matrix<T>::det() const {
    if (this->row != this->col) throw std::invalid_argument("Non-square matrix");

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
        
        det = (c % 2 != 0)? det - data[0][c] * subMatrix.determinant(): det + data[0][c] * subMatrix.determinant();
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
    if (this->row != this->col || this->determinant() == 0) throw std::runtime_error("Matrix is not invertible");

    matrix<T> I(this->row, this->col, "identity");
    augmented_matrix<T> aug(*this, I);
    aug = aug.rref();

    return aug.getRight();
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
    matrix<T> reduced = this->echelonf();
    int rank = 0;
    for (int i = 0; i < this->row; ++i) {
        for (int j = 0; j < this->col; ++j) {
            if (reduced.get(i, j) != T(0)) { ++rank; break; }
        }
    }
    return rank;
}

template <Arithmetic T>
long matrix<T>::null() const { return this->col - this->rank(); }