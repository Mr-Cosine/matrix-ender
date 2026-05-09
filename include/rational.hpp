/*
 * A general-purpose interface for rational numbers
 */

#pragma once
#include <sstream>
#include <concepts.hpp>

template <Ordered T>
inline constexpr bool is_zero(T a);

class rational {
    long num, denom;

public:
    /**
     * Instantiates a fraction based on numbers provided
     * The constructor uses XGCD to simplify the fraction automatically
     *
     * @param numerator Numerator of the fraction
     * @param denominator Denominator of the fraction
     *
     * @return A Rational type
     */
    constexpr rational(long numerator, long denominator) {
        long a = numerator, b = denominator;
        while (a % b != 0) {
            long tmp = a;
            a = b;
            b = tmp % b;
        }
        this->num   = numerator / b;
        this->denom = denominator / b;
    }

    /**
     * Instantiates a fraction based on a string descriptor and a delimiter
     *
     * @param descriptor String description of the fraction
     * @param delimiter Delimiter of the fraction (defaults to '/')
     *
     * @return A Rational type
     */
    rational(std::string, char = '/');

    /**
     * Instantiates a fraction based on an integer
     * (The method is intentionally made non-explicit for compatibility purposes)
     *
     * @param integer Integer to be converted
     *
     * @return A Rational type
     */
    constexpr rational(long num) : num(num), denom(1) {}

    /**
     * Instantiates a zero fraction
     *
     * @return A Rational type
     */
    constexpr rational() : num(0), denom(1) {}

    /**
     * Adds two Rational number
     *
     * @param other Value being added
     *
     * @return A Rational type
     */
    rational operator+(const rational&) const;

    /**
     * Subtract two Rational number (Binary -)
     *
     * @param other Value being subtracted
     *
     * @return A Rational type
     */
    rational operator-(const rational&) const;

    /**
     * Additive inverse
     *
     * @param other Value being subtracted
     *
     * @return A Rational type
     */
    rational operator-() const;

    /**
     * Multiply
     *
     * @param other Multiplier
     *
     * @return A Rational type
     */
    rational operator*(const rational&) const;

    /**
     * Divide
     *
     * @param other Divisor
     *
     * @return A Rational type
     */
    rational operator/(const rational&) const;

    /**
     * Equal
     *
     * @param other Rational being compared
     *
     * @return Boolean
     */
    bool operator==(const rational&) const;

    /**
     * Greater than
     *
     * @param other Rational being compared
     *
     * @return Boolean
     */
    bool operator>(const rational&) const;

    /**
     * Less than
     *
     * @param other Rational being compared
     *
     * @return Boolean
     */
    bool operator<(const rational&) const;

    /**
     * Not equal
     *
     * @param other Rational being compared
     *
     * @return Boolean
     */
    bool operator!=(const rational&) const;

    /**
     * Multiplicative inverse
     *
     * @return A Rational type
     */
    rational inv() const;

    /**
     * Additive inverse (Function-style)
     *
     * @return A Rational type
     */
    rational neg() const;

    /**
     * Stringnify a rational
     *
     * @param delimiter Fraction separator (defaults to '/')
     * 
     * @return String descriptor with a delimiter
     */
    std::string toString(char separator = '/') const {
        std::stringstream ss;
        ss << this->num << separator << this->denom;
        return ss.str();
    }

    double toDouble() const;

    friend inline constexpr bool is_zero<rational>(rational);
};
