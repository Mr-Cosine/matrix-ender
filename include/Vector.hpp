/*
 * Vector utilities
 */

#include <ostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector) {
    os << "[";
    for (size_t i = 0; i < vector.size(); i++) {
        if (i != 0) os << " ";
        os << vector;
        if (i != vector.size() - 1) os << ";\n";
    }
    return os << "]";
}