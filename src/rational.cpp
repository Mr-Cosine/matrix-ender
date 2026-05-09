/*
 * Implementations of rational.hpp
 */
#include "rational.hpp"


#ifdef DEBUG_RATIONAL
#include <iostream>
using namespace std;

inline std::ostream& operator<<(std::ostream& os, const rational& rational) {
    return os << rational.toString();
}

template<typename... Args>
void print(Args... args) {
    ((cout << args << " "), ...);
    cout << endl;
}

int main() {
    print("=== RATIONAL MODULE DEBUG ===");

    // Init
    rational myrat = rational(3, 5);
    rational myrat2 = rational("5/5");
    rational myrat3 = myrat + myrat2;
    rational myrat4 = myrat3 / 2;
    rational myrat5 = rational(4) / rational("12590/1295");

    // Output
    print("myrat1:", myrat);
    print("myrat2:", myrat2);
    print("myrat3:", myrat3);
    print("myrat4:", myrat4);
    print("myrat5:", myrat5);

    return 0;
}
#endif


rational::rational(long numerator, long denominator) {
    long a = numerator, b = denominator;
    while (a % b != 0) {
        long tmp = a;
        a = b;
        b = tmp % b;
    }
    this->num   = numerator / b;
    this->denom = denominator / b;
}

rational::rational(std::string descriptor, char delimiter) : rational(
    std::stoi(descriptor.substr(0, descriptor.find(delimiter))),
    std::stoi(descriptor.substr(descriptor.find(delimiter) + 1, descriptor.size()))
) {}

rational::rational(long num) : num(num), denom(1) {}

rational::rational() : num(0), denom(1) {}

rational rational::operator+(const rational& other) const {
    return rational(
        this->num * other.denom + other.num * this->denom,
        other.denom * this->denom
    );
}

rational rational::operator-(const rational& other) const {
    return rational(
        this->num * other.denom - other.num * this->denom,
        other.denom * this->denom
    );
}

rational rational::operator-() const {
    return rational(
        -this->num,
        this->denom
    );
}

rational rational::operator*(const rational& other) const {
    return rational(
        this->num * other.num,
        this->denom * other.denom
    );
}

rational rational::operator/(const rational& other) const {
    // TODO: Implement division by zero check
    return rational(
        this->num * other.denom,
        this->denom * other.num
    );
}

rational rational::operator+=(const rational& other) const {
    return *this + other;
}

rational rational::operator-=(const rational& other) const {
    return *this - other;
}

rational rational::operator*=(const rational& other) const {
    return *this * other;
}

rational rational::operator/=(const rational& other) const {
    return *this / other;
}

bool rational::operator==(const rational& other) const {
    return this->num == other.num && this->denom == other.denom;
}

bool rational::operator!=(const rational& other) const {
    return this->num != other.num || this->denom != other.denom;
}

rational rational::inv() const {
    return rational(
        this->denom,
        this->num
    );
}

rational rational::neg() const {
    return rational(
        -this->num,
        this->denom
    );
}

double rational::toDouble() const {
    return static_cast<double>(this->num) / static_cast<double>(this->denom);
}