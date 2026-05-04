/*
 * A general-purpose interface for rational numbers
 */

#pragma once
#include <sstream>
#include <ostream>

class Rational {
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
    Rational(long, long);

    /**
     * Instantiates a fraction based on a string descriptor and a delimiter
     *
     * @param descriptor String description of the fraction
     * @param delimiter Delimiter of the fraction (defaults to '/')
     *
     * @return A Rational type
     */
    Rational(std::string, char = '/');

    /**
     * Instantiates a fraction based on an integer
     * (The method is intentionally made non-explicit for compatibility purposes)
     *
     * @param integer Integer to be converted
     *
     * @return A Rational type
     */
    Rational(long);

    /**
     * Adds two Rational number
     *
     * @param other Value being added
     *
     * @return A Rational type
     */
    Rational operator+(const Rational&) const;

    /**
     * Subtract two Rational number
     *
     * @param other Value being subtracted
     *
     * @return A Rational type
     */
    Rational operator-(const Rational&) const;

    /**
     * Multiply two Rational number
     *
     * @param other Multiplier
     *
     * @return A Rational type
     */
    Rational operator*(const Rational&) const;

    /**
     * Subtract
     *
     * @param other Divisor
     *
     * @return A Rational type
     */
    Rational operator/(const Rational&) const;

    /**
     * Find the multiplicative inverse of a Rational
     *
     * @return A Rational type
     */
    Rational inv() const;

    /**
     * Find the additive inverse of a Rational
     *
     * @return A Rational type
     */
    Rational neg() const;

    /**
     * Find the additive inverse of a Rational
     *
     * @param delimiter Fraction separator (defaults to '/')
     * 
     * @return String descriptor with a delimiter
     */
    std::string toString(char = '/') const;
};

std::ostream& operator<<(std::ostream&, const Rational&);