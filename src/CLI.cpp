#include "CLI.hpp"
#include "oext.hpp" // <-- must be included after CLI enum declaration
#include <cstdlib>

#ifdef DEBUG_CLI
using std::cout, std::cin, std::endl;
int main() {
    cout << "===== MATRIX-ENDER TERMINAL (DEBUG) =====\n";
    cout << "MET(v0.1) by A1batr0z & Mr-Cosine | Enter `help` to print help list.\n" << endl;

    add_var("myvar", "[1,2,3;4,5,6]");
    add_var("other", "[1/2, 3/4; 5/6, 7/8]");
    add_var("dec", "[3.14, 5.32; 4.64, 0.24]");

    print_var("myvar");
    print_var("other");
    print_var("dec");

    return 0;
}
#endif

#ifdef DEBUG_CLI_LOOP
using std::cout, std::cin, std::endl;
int main() {
    #ifdef _WIN32
    std::system("cls");
    #else
    std::system("clear");
    #endif

    cout << "===== MATRIX-ENDER TERMINAL (DEBUG) =====\n";
    cout << "MET(v0.1) by A1batr0z & Mr-Cosine | Enter `help` to print help list.\n" << endl;

    std::string user_cmd{};
    while (true) {
        std::cout << "[$user] >>> ";
        std::getline(std::cin, user_cmd);

        if (user_cmd == ":q" || user_cmd == ":quit") break;
        parse_command(user_cmd);
    }

    cout << "\nClosing app...\n" << std::endl;
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

void print_var(std::string identifier, std::ostream& os) {
    auto it = __variables__.find(identifier);
    if (it != __variables__.end()) {
        auto data = it->second->self;
        auto type = it->second->type;
        auto etype = it->second->etype;
        std::visit([&](auto&& arg) {
            if (type == Variable::VarType::PRIMITIVE) {
                os << identifier << type << etype << " => " << arg << std::endl;
            } else {
                os << identifier << type << etype << " =>\n" << arg << std::endl;
            }
        }, data);
    } else {
        os << identifier << " => [undefined]" << std::endl;
    }
}


void parse_command(std::string str) {
    std::vector<std::string> tkns = split(str, ' ');
    std::transform(tkns.begin(), tkns.end(), tkns.begin(), [](std::string s) {
        return removeAll(s, ' ');
    });
    std::string cmd = tkns[0];

    if (cmd == "echo") {

        std::cout << join(std::vector<std::string>(
            tkns.begin() + 1,
            tkns.end()
        ), ' ') << std::endl;

    } else if (cmd == "help") {

        std::cout << std::endl << "=== List of Commands ===\n"
        "echo \t- echo the argument list\n"
        "help \t- show this message\n" << std::endl;

    } else {
        // Other commands

        if (contains(str, '=')) {

            std::vector<std::string> ss = split(str, '=', 1);

            std::transform(ss.begin(), ss.end(), ss.begin(), trim);
            
            // Var name check
            if (containsAny(ss[0], "#%^&*[]:;\"'.,< >")) {
                err("Rejected: Invalid variable identifier. Must not contain '#%^&*[]:;\"'.,< >'");
            }

            add_var(ss[0], trim(ss[1]));

        } else {

            std::cout << std::endl;
            print_var(str);
            std::cout << std::endl;

        }
    }
}

template <typename T>
void evalUpdate(std::string str, T& store) {
    if (__variables__.contains(str)) {
        std::visit([&store](auto&& arg) {
            store += arg;
        }, __variables__[str]->self);
    }
}

void eval(std::string str) {
    str = trim(str);

    size_t post_last_op = 0, i = 0;
    std::vector<std::string> tkns;
    Variable::ExactType prevalence = Variable::ExactType::NUMBER;

    for (size_t i = 0; i < str.size() + 1; i++) {
        if (i == str.size() || contains("+-*/", str[i])) {

            // Validity check
            std::string tkn = trim(str.substr(post_last_op, i - post_last_op + 1));
            if (containsAny(tkn, "#%^&*[]:;\"'.,< >")) {
                std::cout << "[DNE]";
                return;
            } else if (!isNumeric(tkn) && !__variables__.contains(tkn)) {
                std::cout << "[DNE]";
                return;
            }

            // Prevalent type check
            if ((__variables__.contains(tkn)
                && __variables__[tkn]->etype == Variable::ExactType::DECIMAL)
                || isDecimal(str)) {
                prevalence = Variable::ExactType::DECIMAL;
            }

            tkns.push_back(tkn);
            tkns.push_back(std::to_string(str[i]));
            post_last_op = i + 1;
        }
    }


    switch (prevalence) {
        case Variable::ExactType::DECIMAL: {
            long accumulator = 0;
            char oper = '+';

            for (size_t i = 0; i < tkns.size(); i++) {
                if (tkns.size() == 1 && contains("+-*/", tkns[i][0])) {
                    oper = tkns[i][0];
                }

                switch (oper) {
                    case '+':
                        break;
                    case '-':
                        break;
                }
            }
            break;
        }

        case Variable::ExactType::NUMBER: {
            double accumulator = 0;
            break;
        }
    }
}