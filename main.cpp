#include <iostream>
#include <fstream>
#include <sstream>
#include "veb.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Arquivo nao informado\n";
        return 1;
    }

    ifstream entrada(argv[1]);
    if (!entrada.is_open()) {
        cout << "Erro ao abrir arquivo\n";
        return 1;
    }

    VEB arvore(32);

    string linha;
    while (getline(entrada, linha)) {
        if (linha.empty()) continue;

        stringstream ss(linha);
        string op;
        ss >> op;

        if (op == "INC") {
            uint64_t x;
            ss >> x;
            arvore.insert(x);
        }
        else if (op == "REM") {
            uint64_t x;
            ss >> x;
            arvore.remove(x);
        }
        else if (op == "SUC") {
            uint64_t x;
            ss >> x;
            cout << "SUC " << x << "\n";
            int64_t r = arvore.successor(x);
            if (r == -1)
                cout << "+INF\n";
            else
                cout << r << "\n";
        }
        else if (op == "PRE") {
            uint64_t x;
            ss >> x;
            cout << "PRE " << x << "\n";
            int64_t r = arvore.predecessor(x);
            if (r == -1)
                cout << "-INF\n";
            else
                cout << r << "\n";
        }
        else if (op == "IMP") {
            cout << "IMP\n";
            arvore.print();
        }
    }

    return 0;
}
