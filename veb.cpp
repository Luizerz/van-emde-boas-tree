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
    if (min == -1)
        return;
    if (min == max) {
        if ((int64_t)x == min)
            min = max = -1;
        return;
    }
    if (w == 1) {
        // universo {0, 1} com os dois presentes: sobra o outro
        if (x == 0) min = 1;
        else max = 0;
        return;
    }
    if ((int64_t)x < min)
        return;  // não está na estrutura
    if ((int64_t)x == min) {
        // o novo min é o menor elemento do primeiro cluster;
        // ele é promovido e removido do cluster logo abaixo
        uint64_t primeiro = (uint64_t)summary->min;
        VEB* pc = clusters.find((uint32_t)primeiro);
        x = index(primeiro, (uint64_t)pc->min);
        min = (int64_t)x;
    }

    uint64_t h = high(x), l = low(x);
    VEB* c = clusters.find((uint32_t)h);
    if (c == nullptr)
        return;  // não está na estrutura
    c->remove(l);

    if (c->min == -1) {
        // cluster esvaziou: sai da tabela e do summary (espaço linear)
        clusters.remove((uint32_t)h);
        delete c;
        summary->remove(h);
        if (summary->min == -1) {
            delete summary;
            summary = nullptr;
        }
        if ((int64_t)x == max) {
            if (summary == nullptr) {
                max = min;
            } else {
                uint64_t ultimo = (uint64_t)summary->max;
                VEB* uc = clusters.find((uint32_t)ultimo);
                max = (int64_t)index(ultimo, (uint64_t)uc->max);
            }
        }
    }
    else if ((int64_t)x == max) {
        max = (int64_t)index(h, (uint64_t)c->max);
    }
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
    if (min == -1) {
        std::cout << "\n";  // estrutura vazia: linha de conteúdo em branco
        return;
    }
    std::cout << "Min: " << min;
    if (summary != nullptr) {
        // clusters em ordem crescente de índice, via summary
        for (int64_t h = summary->min; h != -1; h = summary->successor((uint64_t)h)) {
            VEB* c = clusters.find((uint32_t)h);
            std::cout << ", C[" << h << "]: " << c->min;
            for (int64_t l = c->successor((uint64_t)c->min); l != -1; l = c->successor((uint64_t)l))
                std::cout << ", " << l;
        }
    }
    std::cout << "\n";
}
