#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "matrix.hpp"

template <typename T>
matrix<T> make_matrix(const std::vector<std::vector<T>>& values) {
    matrix<T> result(static_cast<long>(values.size()), values.empty() ? 0 : static_cast<long>(values[0].size()));
    for (long r = 0; r < static_cast<long>(values.size()); ++r) {
        for (long c = 0; c < static_cast<long>(values[r].size()); ++c) {
            result.put(r, c, values[r][c]);
        }
    }
    return result;
}

template <typename T>
void assert_equal(const T& actual, const T& expected, const std::string& label) {
    if (!(actual == expected)) {
        std::cerr << "FAILED: " << label << " (actual=" << actual << ", expected=" << expected << ")\n";
        std::exit(EXIT_FAILURE);
    }
}

void assert_equal(double actual, double expected, const std::string& label) {
    const double tolerance = 1e-9;
    if (std::fabs(actual - expected) > tolerance) {
        std::cerr << "FAILED: " << label << " (actual=" << actual << ", expected=" << expected << ")\n";
        std::exit(EXIT_FAILURE);
    }
}

template <typename T>
void assert_matrix_equal(const matrix<T>& actual, const matrix<T>& expected, const std::string& label) {
    if (actual.getRow() != expected.getRow() || actual.getCol() != expected.getCol()) {
        std::cerr << "FAILED: " << label << " (dimension mismatch)\n";
        std::exit(EXIT_FAILURE);
    }

    for (long r = 0; r < actual.getRow(); ++r) {
        for (long c = 0; c < actual.getCol(); ++c) {
            if constexpr (std::is_same_v<T, double>) {
                assert_equal(actual.get(r, c), expected.get(r, c), label + "[" + std::to_string(r) + "," + std::to_string(c) + "]");
            } else {
                assert_equal(actual.get(r, c), expected.get(r, c), label + "[" + std::to_string(r) + "," + std::to_string(c) + "]");
            }
        }
    }
}

void test_put_get_and_string_constructor() {
    matrix<int> m(2, 2);
    m.put(0, 0, 1);
    m.put(0, 1, 2);
    m.put(1, 0, 3);
    m.put(1, 1, 4);

    assert_equal(m.get(0, 0), 1, "put/get (0,0)");
    assert_equal(m.get(0, 1), 2, "put/get (0,1)");
    assert_equal(m.get(1, 0), 3, "put/get (1,0)");
    assert_equal(m.get(1, 1), 4, "put/get (1,1)");

    matrix<int> fromString("1,2;3,4");
    assert_matrix_equal(fromString, m, "string constructor");
}

void test_arithmetic_operations() {
    matrix<int> a("1,2;3,4");
    matrix<int> b("5,6;7,8");

    matrix<int> expectedAdd = make_matrix<int>({{6, 8}, {10, 12}});
    assert_matrix_equal(a + b, expectedAdd, "matrix addition");

    matrix<int> expectedSub = make_matrix<int>({{-4, -4}, {-4, -4}});
    assert_matrix_equal(a - b, expectedSub, "matrix subtraction");

    matrix<int> expectedMul = make_matrix<int>({{19, 22}, {43, 50}});
    assert_matrix_equal(a * b, expectedMul, "matrix multiplication");

    matrix<int> scaled = a * 2;
    matrix<int> expectedScaled = make_matrix<int>({{2, 4}, {6, 8}});
    assert_matrix_equal(scaled, expectedScaled, "scalar multiplication");

    a *= 3;
    matrix<int> expectedScaledInPlace = make_matrix<int>({{3, 6}, {9, 12}});
    assert_matrix_equal(a, expectedScaledInPlace, "scalar multiplication in-place");
}

void test_transpose_and_determinant() {
    matrix<int> m("1,2;3,4");
    matrix<int> expectedTranspose = make_matrix<int>({{1, 3}, {2, 4}});
    assert_matrix_equal(m.transpose(), expectedTranspose, "transpose");
    assert_equal(m.det(), -2, "determinant");
}

void test_ref_and_rref() {
    matrix<double> m("1,2,3;4,5,6");
    matrix<double> expectedRef = make_matrix<double>({{1.0, 2.0, 3.0}, {0.0, -3.0, -6.0}});
    matrix<double> expectedRref = make_matrix<double>({{1.0, 0.0, -1.0}, {0.0, 1.0, 2.0}});

    assert_matrix_equal(m.ref(), expectedRef, "row echelon form");
    assert_matrix_equal(m.rref(), expectedRref, "reduced row echelon form");
}

int main() {
    std::cout << "Running matrix test suite...\n";

    test_put_get_and_string_constructor();
    std::cout << "  [PASS] put/get and string constructor\n";

    test_arithmetic_operations();
    std::cout << "  [PASS] arithmetic operations\n";

    test_transpose_and_determinant();
    std::cout << "  [PASS] transpose and determinant\n";

    test_ref_and_rref();
    std::cout << "  [PASS] ref and rref\n";

    std::cout << "All matrix tests passed.\n";
    return EXIT_SUCCESS;
}
