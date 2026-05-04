/*
 * Implementations of Rational.hpp
 */
#include "Rational.hpp"
#include <iostream>

using std::cout, std::endl;

Rational::Rational(long numerator, long denominator) {
    long a = numerator, b = denominator;
    while (a % b != 0) {
        long tmp = a;
        a = b;
        b = tmp % b;
    }
    this->num   = numerator / b;
    this->denom = denominator / b;
}

Rational::Rational(std::string descriptor, char delimiter) : Rational(
    std::stoi(descriptor.substr(0, descriptor.find(delimiter))),
    std::stoi(descriptor.substr(descriptor.find(delimiter) + 1, descriptor.size()))
) {}

Rational::Rational(long num) : num(num), denom(1) {}

Rational Rational::operator+(const Rational& other) const {
    return Rational(
        this->num * other.denom + other.num * this->denom,
        other.denom * this->denom
    );
}

Rational Rational::operator-(const Rational& other) const {
    return Rational(
        this->num * other.denom - other.num * this->denom,
        other.denom * this->denom
    );
}

Rational Rational::operator*(const Rational& other) const {
    return Rational(
        this->num * other.num,
        this->denom * other.denom
    );
}

Rational Rational::operator/(const Rational& other) const {
    return Rational(
        this->num * other.denom,
        this->denom * other.num
    );
}

Rational Rational::inv() const {
    return Rational(
        this->denom,
        this->num
    );
}

Rational Rational::neg() const {
    return Rational(
        -this->num,
        this->denom
    );
}

std::string Rational::toString(char separator) const {
    std::stringstream ss;
    ss << this->num << separator << this->denom;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Rational& rational) {
    return (os << rational.toString());
}

#ifdef DEBUG_RATIONAL
#include <iostream>
using namespace std;

template<typename... Args>
void print(Args... args) {
    ((cout << args << " "), ...);
    cout << endl;
}

int main() {
    print("=== RATIONAL MODULE DEBUG ===");

    // Init
    Rational myrat = Rational(3, 5);
    Rational myrat2 = Rational("5/5");
    Rational myrat3 = myrat + myrat2;
    Rational myrat4 = myrat3 / 2;
    Rational myrat5 = Rational(4) / Rational("12590/1295");

    // Output
    print("myrat1:", myrat);
    print("myrat2:", myrat2);
    print("myrat3:", myrat3);
    print("myrat4:", myrat4);
    print("myrat5:", myrat5);

    return 0;
}
#endif