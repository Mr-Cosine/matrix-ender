#ifndef MATRIX_TPP
#define MATRIX_TPP

#include "augMatrix.h"

template <typename T>
std::vector<std::vector<T>> matrix<T>::solve(std::vector<T> b) const {
    matrix<T> matrixb(rowNum, 1);
    for (int i = 0; i < rowNum; ++i)
        matrixb.put(i, 0, b[i]);
    augMatrix<T> aug(*this, matrixb);

    std::vector<std::vector<T>> sol = aug.solve();
    return sol;
}

template <typename T>
bool matrix<T>::inSpan(std::vector<T> b) const {
    matrix<T> matrixb(rowNum, 1);
    for (int i = 0; i < rowNum; ++i)
        matrixb.put(i, 0, b[i]);
    augMatrix<T> aug(*this, matrixb);

    try {
        std::vector<std::vector<T>> sol = aug.solve();
    }
    catch(const std::runtime_error& e) { return false; }

    return true;
}

#endif