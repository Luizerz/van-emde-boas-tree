#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstdint>
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

    // Índice auxiliar para a operação IMP: a impressão lista os valores de
    // cada cluster na ordem em que foram incluídos (INC). A vEB não guarda
    // essa ordem, então mantemos o número de sequência de inclusão de cada
    // valor presente (espaço O(n), igual ao da estrutura).
    unordered_map<uint32_t, uint64_t> seq;
    uint64_t seqCounter = 0;

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
            uint32_t k = (uint32_t)x;
            if (seq.find(k) == seq.end())
                seq[k] = seqCounter++;
        }
        else if (op == "REM") {
            uint64_t x;
            ss >> x;
            arvore.remove(x);
            seq.erase((uint32_t)x);
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
            if (arvore.min == -1)
                continue;  // estrutura vazia: nada a imprimir além do IMP

            cout << "Min: " << arvore.min;

            // agrupa os valores presentes (menos o min) por cluster (high16)
            uint32_t mn = (uint32_t)arvore.min;
            unordered_map<uint32_t, vector<pair<uint64_t, uint32_t>>> grupos;
            for (const auto& kv : seq) {
                uint32_t v = kv.first;
                if (v == mn) continue;
                grupos[v >> 16].push_back({kv.second, v & 0xFFFFu});
            }

            vector<uint32_t> highs;
            highs.reserve(grupos.size());
            for (const auto& g : grupos) highs.push_back(g.first);
            sort(highs.begin(), highs.end());

            for (uint32_t h : highs) {
                auto& vec = grupos[h];
                sort(vec.begin(), vec.end());  // por seq de inclusão
                cout << ", C[" << h << "]: " << vec[0].second;
                for (size_t i = 1; i < vec.size(); i++)
                    cout << ", " << vec[i].second;
            }
            cout << "\n";
        }
    }

    return 0;
}
