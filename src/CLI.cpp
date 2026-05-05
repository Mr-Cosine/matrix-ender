#include "CLI.hpp"
#include "oext.hpp" // <-- must be included after CLI enum declaration


#ifdef DEBUG_CLI
using std::cout, std::cin, std::endl;
int main() {
    cout << "===== MATRIX-ENDER TERMINAL (DEBUG) =====\n";
    cout << "MET(v0.0) by A1batr0z & Mr-Cosine | Enter `help` to get help list.\n" << endl;

    add_var("myvar", "[1,2,3;4,5,6]");
    add_var("other", "[1/2, 3/4; 5/6, 7/8]");
    add_var("dec", "[3.14, 5.32; 4.64, 0.24]");

    print_var("myvar");
    print_var("other");
    print_var("dec");

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
            size_t start = var.find_first_of('[');
            size_t fcom = var.find_first_of(','), fsemc = var.find_first_of(';');
            tkn = var.substr(var.find_first_of('[') + 1, (fcom < fsemc ? fcom : fsemc) - start - 1);
            break;
        }

        default: {
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
                    this->self = matrix<long>(var);
                    break;

                case ExactType::DECIMAL:
                    this->self = matrix<double>(var);
                    break;

                case ExactType::RATIONAL:
                    this->self = matrix<rational>(var);
                    break;

                // No string matrices...
                default:
                    this->self = std::monostate{};
                    return;
            }
            break;

        case VarType::PRIMITIVE:
            switch (this->etype) {
                case ExactType::NUMBER:
                    this->self = std::stol(var);
                    break;

                case ExactType::DECIMAL:
                    this->self = std::stod(var);
                    break;

                case ExactType::RATIONAL:
                    this->self = rational(var);
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
                && tkn.find('/') == std::string::npos) {
        for (const char& c: tkn) {
            if (c - '0' < 0 || c - '0' > 9) {
                return ExactType::VOID;
            }
        }
        return ExactType::NUMBER;
    }
    return ExactType::VOID;
}

Variable::VarType Variable::var_type_of(std::string token) {
    token = removeAll(token, ' ');
    bool isMatrix = token.find('[') != std::string::npos
                    && token.find(']') != std::string::npos
                    && (token.find(',') != std::string::npos
                    || token.find(';') != std::string::npos);
    if (isMatrix) {
        return VarType::MATRIX;
    } else {
        return VarType::PRIMITIVE;
    }
}

void add_var(std::string identifier, std::string variable_string) {
    // if (__variables__.contains(identifier)) return;

    Variable* raw_ptr = new Variable(variable_string);
    __variables__.insert_or_assign(identifier, raw_ptr);
}

void print_var(std::string identifier) {
    auto it = __variables__.find(identifier);
    if (it != __variables__.end()) {
        auto data = it->second->self;
        auto type = it->second->type;
        auto etype = it->second->etype;
        std::visit([&](auto&& arg) {
            std::cout << identifier << type << etype << " =>\n" << arg << std::endl;
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
