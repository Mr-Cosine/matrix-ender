#include "Matrix.hpp"

#include <iostream>

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
    : row(row), col(column), matrix(row, std::vector<T>(column, 0)) {
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

            case FillType::LOWER_TRI:
                for (int i = 0; i < column; i++) {
                    for (int j = 0; j < row - i; j++) {
                        this->matrix[row - 1 - j][i] = filler;
                    }
                }
                break;

            default:
                throw InvalidFillTypeException("fill_type does not conform to any FillType enum");
        }
}

template <Arithmetic T>
void Matrix<T>::print(char row_delimiter, char column_delimiter) const {
    std::cout << "[";
    for (int i = 0; i < this->row; i++) {
        for (int j = 0; j < this->col; j++) {
            if (i != 0 && j == 0) std::cout << " ";
            std::cout << this->matrix[i][j];
            if (j != this->col - 1) std::cout << column_delimiter;
        }
        if (i != this->row - 1) std::cout << row_delimiter;
    }
    std::cout << "]" << std::endl;
}


#ifdef DEBUG_MATRIX
int main() {
    print("=== MATRIX MODULE DEBUG ===\n");

    Matrix<int> mymat = Matrix(4, 4, 1, FillType::LOWER_TRI);
    mymat.print();

    return 0;
}
#endif