#include "Matrix.hpp"
#include "Rational.hpp"
#include "Useful.hpp"

#include <iostream>
#include <sstream>

#ifdef DEBUG_MATRIX
using std::cout, std::cin, std::endl;

// Print helper
template <typename... Args>
void print(Args... args) {
    ((cout << args << " "), ...);
    cout << endl;
}

#endif

template <Arithmetic T>
Matrix<T>::Matrix(long row, long column, T filler, FillType fill_type)
    : row(row), col(column), matrix(row, std::vector<T>(column, T(0))) {
        switch (fill_type) {
            case FillType::EVERY:
                this->matrix = std::vector<std::vector<T>>(row, std::vector<T>(column, filler));
                break;

            case FillType::UPPER_TRI:
                for (int i = 0; i < column; i++) {
                    for (int j = 0; j <= i; j++) {
                        this->matrix[j][i] = filler;
                    }
                }
                break;
            
            case FillType::UPPER_TRI_R:
                for (int i = 0; i < column; i++) {
                    for (int j = 0; j < row - i; j++) {
                        this->matrix[j][i] = filler;
                    }
                }
                break;

            case FillType::LOWER_TRI:
                for (int i = 0; i < column; i++) {
                    for (int j = 0; j < row - i; j++) {
                        this->matrix[row - 1 - j][i] = filler;
                    }
                }
                break;
            
            case FillType::LOWER_TRI_R:
                for (int i = 0; i < column; i++) {
                    for (int j = 0; j <= i; j++) {
                        this->matrix[row - 1 - j][i] = filler;
                    }
                }
                break;

            default:
                throw InvalidFillTypeException("fill_type does not conform to any FillType enum");
        }
}

template <Arithmetic T>
void Matrix<T>::print(char row_delimiter, char column_delimiter, bool tab, bool pad) const {
    std::cout << (pad ? "\n[" : "[");
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            if ( i != 0 && j == 0) std::cout << " ";
            std::cout << this->matrix[i][j];
            if (j != this->col - 1) std::cout << column_delimiter;
            if (tab && !(i == this->row - 1 && j == this->col - 1)) std::cout << "\t";
        }
        if (i != this->row - 1) std::cout << row_delimiter;
    }
    std::cout << (pad ? "]\n" : "]") << std::endl;
}

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
        this->matrix.push_back(row);
    }
    this->row = this->matrix.size();
    this->col = this->matrix[0].size();
}

template <Arithmetic T>
std::string Matrix<T>::toString(long row, long column, char row_delimiter, char column_delimiter, bool tab) const {
    if (row > this->row || column > this->col) {
        throw IndexOutOfBoundException("Index out of bounds");
    }

    row = row < 0 ? this->row : row;
    column = column < 0 ? this->col : col;
    std::ostringstream oss;
    oss << "[";
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < this->col; j++) {
            if (i != 0 && j == 0) oss << " ";
            oss << this->matrix[i][j];
            if (j != this->col - 1) {
                oss << column_delimiter;
                if (tab) oss << "\t";
            }
        }
        if (i != column - 1) oss << row_delimiter;
    }
    oss << "]" << std::endl;
    return oss.str();
}

template <Arithmetic T>
std::ostream& operator<<(std::ostream& os, const Matrix<T> matrix) {
    return os << matrix.toString();
}


#ifdef DEBUG_MATRIX
int main() {
    
    print("=== MATRIX MODULE DEBUG ===\n");

    Matrix<int> mymat(4, 4, 1, FillType::UPPER_TRI_R);
    mymat.print();
    Matrix<Rational> myratmat(4, 4, 2, FillType::LOWER_TRI);
    myratmat.print();
    Matrix<int> strmat("1,2,3;4,5,6;7,8,9,;");
    strmat.print();
    cout << "printing matrix:\n" << strmat << endl;

    return 0;
}
#endif