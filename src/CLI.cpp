#include <iostream>
#include "CLI.hpp"

#ifdef DEBUG_CLI
using std::cout, std::cin, std::endl;

int main() {
    cout << "===== MATRIX-ENDER TERMINAL (DEBUG) =====\n";
    cout << "MET(v0.0) by A1batr0z & Mr-Cosine | Enter `help` to get help list.\n" << endl;

    add_var("myvar", "[1,2,3;4,5,6]");
    print_var("myvar");

    return 0;
}
#endif

Variable::Variable() : self(std::monostate{}), etype(ExactType::VOID), type(VarType::PRIMITIVE) {};
Variable::Variable(std::string var) {
    var = removeAll(var, ' ');
    this->type = var_type_of(var);
    std::string tkn;

    switch (this->type) {
        case VarType::MATRIX: {
            size_t fcom = var.find_first_of(','), fsemc = var.find_first_of(';');
            tkn = var.substr(var.find_first_of('['), fcom < fsemc ? fcom : fsemc);
            break;
        }

        case VarType::VECTOR: {
            tkn = delimitBy(tkn, '[', ';');
            break;
        }

        case VarType::PRIMITIVE: {
            tkn = var;
            break;
        }
    }

    this->etype = exact_type_of(tkn);
    if (this->etype == ExactType::VOID) {
        this->self = std::monostate{};
        return;
    }

    switch (this->type) {
        case VarType::MATRIX:
            switch (this->etype) {
                case ExactType::NUMBER:
                    this->self = Matrix<long>(var);
                    break;

                case ExactType::DECIMAL:
                    this->self = Matrix<double>(var);
                    break;

                case ExactType::RATIONAL:
                    this->self = Matrix<Rational>(var);
                    break;

                // No string matrices...
                default:
                    this->self = std::monostate{};
                    return;
            }
            break;
        case VarType::VECTOR:
            switch (this->etype) {
                /*
                TODO: Implement parse_vector<T>(std::string);

                case ExactType::NUMBER:
                    break;
                case ExactType::DECIMAL:
                    break;

                case ExactType::RATIONAL:
                    break;
                */

                // No string vectors...
                default:
                    this->self = std::monostate{};
                    return;
            }
            break;
        case VarType::PRIMITIVE:
            switch (this->etype) {
                case ExactType::NUMBER:
                    this->self = static_cast<long>(std::stoi(var));
                    break;

                case ExactType::DECIMAL:
                    this->self = std::stod(var);
                    break;

                case ExactType::RATIONAL:
                    this->self = Rational(var);
                    break;

                case ExactType::STRING:
                    this->self = var;

                default:
                    this->self = std::monostate{};
                    return;
            }
            break;
    }
}

Variable::ExactType Variable::exact_type_of(std::string tkn) {
    if (count(tkn, '"') == 2) {
        return ExactType::STRING;
    } else if (tkn.find('.') != std::string::npos && tkn.find('/') == std::string::npos) {
        return ExactType::DECIMAL;
    } else if (tkn.find('/') != std::string::npos && tkn.find('.') == std::string::npos) {
        return ExactType::RATIONAL;
    } else if (tkn.find('"') == std::string::npos
                && tkn.find('.') == std::string::npos
                && tkn.find('/') != std::string::npos) {
        for (const char& digit: "1234567890") {
            if (tkn.find_first_not_of(digit) != std::string::npos) {
                return ExactType::VOID;
            }
        }
        return ExactType::NUMBER;
    }
    return ExactType::VOID;
}

Variable::VarType Variable::var_type_of(std::string token) {
    token = removeAll(token, ' ');
    bool isVector = token.find('[') != std::string::npos
                    && token.find(']') != std::string::npos
                    && token.find(',') != std::string::npos;
    bool isMatrix = token.find('[') != std::string::npos
                    && token.find(']') != std::string::npos
                    && token.find(',') != std::string::npos
                    && token.find(';') != std::string::npos;
    if (isMatrix) {
        /*
        size_t start = token.find_first_of('[');
        size_t comma = token.find_first_of(',');
        size_t semicol = token.find_first_of(';');
        size_t end = comma < semicol ? comma : semicol;
        std::string entry = token.substr(start, end);
        */
        return VarType::MATRIX;
    } else if (isVector) {
        /*
        size_t start = token.find_first_of('[');
        size_t end = token.find_first_of(';');
        std::string entry = token.substr(start, end);
        */
        return VarType::VECTOR;
    } else {
        return VarType::PRIMITIVE;
    }
}

void add_var(std::string identifier, std::string variable_string) {
    // if (__variables__.contains(identifier)) return;

    Variable* raw_ptr = new Variable(variable_string);
    __variables__.insert_or_assign(identifier, raw_ptr);
}

// << overload
std::ostream& operator<<(std::ostream& os, const std::monostate& _) {
    return os << "<void>";
}

std::ostream& operator<<(std::ostream& os, const Variable::VarType& type) {
    switch (type) {
        case Variable::VarType::MATRIX:
            return os << "[Matrix]";
        case Variable::VarType::VECTOR:
            return os << "[Vector]";
        case Variable::VarType::PRIMITIVE:
            return os << "[Primitive]";
        default:
            return os << "[VOID]";
    }
}

std::ostream& operator<<(std::ostream& os, const Variable::ExactType& type) {
    switch (type) {
        case Variable::ExactType::NUMBER:
            return os << "<number>";
        case Variable::ExactType::DECIMAL:
            return os << "<decimal>";
        case Variable::ExactType::RATIONAL:
            return os << "<rational>";
        case Variable::ExactType::STRING:
            return os << "<string>";
        case Variable::ExactType::VOID:
            return os << "<void>";
    }
}

void print_var(std::string identifier) {
    auto it = __variables__.find(identifier);
    if (it != __variables__.end()) {
        auto data = it->second->self;
        auto type = it->second->type;
        auto etype = it->second->etype;
        std::visit([&](auto&& arg) {
            std::cout << identifier << type << etype << " => " << arg << std::endl;
        }, data);
    } else {
        std::cout << identifier << " => " << "[undefined]" << std::endl;
    }
}

/*
TODO: Implement

std::string parse_command(std::string cmd) {
    std::istringstream iss(cmd);
    std::string cmd_tkn;
    std::getline(iss, cmd_tkn, ' ');

    if (cmd_tkn == "let") {
        // Variable definition
        

    } else {
        // Variable retrieval

    }
}
*/

// Helper functions

// Bidirectional trimming
std::string trim(std::string str) {
    size_t bgn, end;
    for (bgn = 0; !std::isspace(str[bgn]); bgn++);
    for (end = str.size() - 1; !std::isspace(str[end]); end--);
    return str.substr(bgn, end + 1);
}

// Global removal
std::string removeAll(std::string str, char which) {
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == which) str.erase(i, 1);
    }
    return str;
};

size_t count(std::string haystack, char needle) {
    size_t count = 0;
    for (const char& c: haystack) {
        if (c == needle) count++;
    }
    return count;
}

// Greedy search
std::string delimitBy(std::string str, char from, char to) {
    size_t start = str.find_first_of(from);
    size_t end = str.find_last_of(to);
    return str.substr(start, end + 1);
}