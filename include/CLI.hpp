/*
 * The command line interface
 */

#pragma once

#include <unordered_map>
#include <string>
#include <variant>
#include <sstream>
#include <memory>
#include <iostream>
#include <algorithm>

#include "matrix.hpp"
#include "rational.hpp"
#include "util.hpp"

// Variable serialization
struct Variable {
    enum class VarType {
        MATRIX,
        PRIMITIVE
    };

    enum class ExactType {
        NUMBER,
        STRING,
        RATIONAL,
        DECIMAL,
        VOID
    };

    using VarStore = std::variant<
        matrix<double>,
        matrix<long>,
        matrix<rational>,
        std::vector<double>,
        std::vector<long>,
        std::vector<rational>,
        double, long, rational, std::string, std::monostate
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
void print_var(std::string, std::ostream& = std::cout);

// Command parsing
void parse_command(std::string);
