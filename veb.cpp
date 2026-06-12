#include <iostream>
#include "veb.h"

VEB::VEB(int w) : w(w), min(-1), max(-1), summary(nullptr) {}

VEB::~VEB() {
    if (summary != nullptr) {
        // O summary enumera exatamente os clusters presentes na tabela.
        for (int64_t h = summary->min; h != -1; h = summary->successor((uint64_t)h))
            delete clusters.find((uint32_t)h);
        delete summary;
    }
}

void VEB::insert(uint64_t x) {
    if (min == -1) {
        min = max = (int64_t)x;
        return;
    }
    if ((int64_t)x == min || (int64_t)x == max)
        return;
    if ((int64_t)x < min) {
        // x vira o novo min e o antigo min desce para os clusters
        uint64_t antigo = (uint64_t)min;
        min = (int64_t)x;
        x = antigo;
    }
    if (w > 1) {
        uint64_t h = high(x), l = low(x);
        VEB* c = clusters.find((uint32_t)h);
        if (c == nullptr) {
            c = new VEB(w / 2);
            clusters.insert((uint32_t)h, c);
        }
        if (c->min == -1) {
            // cluster vazio: inserção O(1) + registro no summary
            if (summary == nullptr)
                summary = new VEB(w / 2);
            summary->insert(h);
            c->min = c->max = (int64_t)l;
        } else {
            c->insert(l);
        }
    }
    if ((int64_t)x > max)
        max = (int64_t)x;
}

void VEB::remove(uint64_t x) {
    // TODO
}

int64_t VEB::successor(uint64_t x) const {
    if (w == 1)
        return (x == 0 && max == 1) ? 1 : -1;
    if (min != -1 && (int64_t)x < min)
        return min;

    uint64_t h = high(x), l = low(x);
    VEB* c = clusters.find((uint32_t)h);
    if (c != nullptr && c->max != -1 && (int64_t)l < c->max) {
        // o sucessor está dentro do mesmo cluster
        int64_t offset = c->successor(l);
        return (int64_t)index(h, (uint64_t)offset);
    }
    // senão, é o min do próximo cluster não vazio
    int64_t proximo = (summary != nullptr) ? summary->successor(h) : -1;
    if (proximo == -1)
        return -1;
    VEB* pc = clusters.find((uint32_t)proximo);
    return (int64_t)index((uint64_t)proximo, (uint64_t)pc->min);
}

int64_t VEB::predecessor(uint64_t x) const {
    if (w == 1)
        return (x == 1 && min == 0) ? 0 : -1;
    if (max != -1 && (int64_t)x > max)
        return max;

    uint64_t h = high(x), l = low(x);
    VEB* c = clusters.find((uint32_t)h);
    if (c != nullptr && c->min != -1 && (int64_t)l > c->min) {
        // o predecessor está dentro do mesmo cluster
        int64_t offset = c->predecessor(l);
        return (int64_t)index(h, (uint64_t)offset);
    }
    // senão, é o max do cluster não vazio anterior
    int64_t anterior = (summary != nullptr) ? summary->predecessor(h) : -1;
    if (anterior == -1) {
        // o min deste nó não está nos clusters: pode ser o predecessor
        if (min != -1 && (int64_t)x > min)
            return min;
        return -1;
    }
    VEB* pc = clusters.find((uint32_t)anterior);
    return (int64_t)index((uint64_t)anterior, (uint64_t)pc->max);
}

void VEB::print() const {
    // TODO
}
