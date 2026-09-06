#include <iostream>
#include <string>
#include "oext.hpp"

#ifndef PYTC
#define PYTC -1
#endif

using namespace std;

template <typename... Args>
void print(Args... args) {
    ((cout << args << " "), ...);
    // cout << endl;
}

int main(int argc, char* argv[]) {
    switch (PYTC) {
        case 0: {
            // Addition = 0, expected input count == 2
            // print("argc:", argc);
            if (argc != 3) return 1;
            string a(argv[1]), b(argv[2]);
            matrix<double> ma(a), mb(b);
            print((ma + mb).toString(-1, -1, ';', ',', false, false));
            break;
        }
        
        case 1: {
            // Subtraction = 1, expected input count == 2
            if (argc != 3) return 1;
            string a(argv[1]), b(argv[2]);
            matrix<double> ma(a), mb(b);
            print((ma - mb).toString(-1, -1, ';', ',', false, false));
            break;
        }

        case 2: {
            // Multiplication = 2, expected input count == 2
            if (argc != 3) return 1;
            string a(argv[1]), b(argv[2]);
            matrix<double> ma(a), mb(b);
            print((ma * mb).toString(-1, -1, ';', ',', false, false));
            break;
        }

        case 3: {
            // RREF = 3, expected input count == 1
            if (argc != 2) return 1;
            string a(argv[1]);
            matrix<double> ma(a);
            print(ma.rref().toString(-1, -1, ';', ',', false, false));
            break;
        }

        case 4: {
            // Determinant = 4, expected input count == 1
            if (argc != 2) return 1;
            string a(argv[1]);
            matrix<double> ma(a);
            print(ma.det());
            break;
        }
        
        case 5: {
            // Tranpose = 5, expected input count == 1
            if (argc != 2) return 1;
            string a(argv[1]);
            matrix<double> ma(a);
            print(ma.transpose());
            break;
        }
        
        default: {
            // No matching cases
            print("No matching cases! Aborting...");
            return 1;
        }
    }
}
