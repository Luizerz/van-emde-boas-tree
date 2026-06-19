#ifndef VEB_H
#define VEB_H

#include <cstdint>
#include "hashtable.h"

// Nó de uma árvore van Emde Boas para um universo de 2^w valores.
// O min não é armazenado recursivamente nos clusters, e tanto o summary
// quanto os clusters são criados sob demanda (lazy) na tabela de
// dispersão, o que garante espaço O(n) no número de elementos.
struct VEB {
    int w;            // largura em bits do universo (universo = 2^w)
    int64_t min;      // -1 indica estrutura vazia
    int64_t max;
    VEB* summary;     // vEB de w/2 bits com os índices dos clusters não vazios
    HashTable clusters;

    explicit VEB(int w);
    ~VEB();

    void insert(uint64_t x);                 // INC
    void remove(uint64_t x);                 // REM
    int64_t successor(uint64_t x) const;     // SUC (-1 = +INF)
    int64_t predecessor(uint64_t x) const;   // PRE (-1 = -INF)

private:
    uint64_t high(uint64_t x) const { return x >> (w / 2); }
    uint64_t low(uint64_t x) const { return x & ((1ULL << (w / 2)) - 1); }
    uint64_t index(uint64_t h, uint64_t l) const { return (h << (w / 2)) | l; }
};

#endif
