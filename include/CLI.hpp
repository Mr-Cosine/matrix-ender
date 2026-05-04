#pragma once
#include <unordered_map>
#include <string>
#include <variant>
#include <strstream>
#include <memory>

#include "Matrix.hpp"
#include "Rational.hpp"

// Variable serialization
struct Variable {
    enum class VarType {
        MATRIX,
        VECTOR,
        PRIMITIVE
    };

    enum class ExactType {
        NUMBER,
        STRING,
        RATIONAL,
        DECIMAL,
        VOID
    };

    /*
    using _matrix = std::variant<
        Matrix<int>,
        Matrix<float>,
        Matrix<double>,
        Matrix<Rational>,
        Matrix<long>,
        Matrix<short>
    >;

    using _vector = std::variant<
        std::vector<int>,
        std::vector<float>,
        std::vector<double>,
        std::vector<Rational>,
        std::vector<long>,
        std::vector<short>
    >;

    using _primi = std::variant<int, float, double, Rational, long, short, char, std::string, std::monostate>;
    */

    using VarStore = std::variant<
        Matrix<double>,
        Matrix<long>,
        Matrix<Rational>,
        std::vector<double>,
        std::vector<long>,
        std::vector<Rational>,
        double, long, Rational, std::string, std::monostate
    >;

    VarStore self;
    ExactType etype;
    VarType type;

    Variable();
    Variable(std::string var);

    static ExactType exact_type_of(std::string);
    static VarType var_type_of(std::string);
};

inline std::unordered_map<std::string, Variable*> __variables__;

// Add var
void add_var(std::string, std::string);

// Print var
void print_var(std::string);

// Command parsing
std::string parse_command(std::string);

// Bidirectional trimming
std::string trim(std::string);

// Global removal
std::string removeAll(std::string, char);

// Count char occurrences
size_t count(std::string, char);

// Greedy search
std::string delimitBy(std::string, char, char);
