#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cstdint>
#include <cstddef>

struct VEB;

// Tabela de dispersão com endereçamento aberto (sondagem linear) e
// table doubling/halving. Mapeia índice de cluster (uint32_t) -> VEB*.
// É ela que garante o espaço linear da van Emde Boas: só os clusters
// não vazios ocupam memória.
class HashTable {
private:
    enum SlotState : uint8_t { EMPTY, OCCUPIED, DELETED };

    struct Slot {
        uint32_t key;
        VEB* value;
        SlotState state;
    };

    Slot* slots;
    size_t capacity;  // sempre potência de 2 (0 antes da primeira inserção)
    int lg;           // log2(capacity)
    size_t count;     // slots OCCUPIED
    size_t used;      // slots OCCUPIED + DELETED (controla a reconstrução)

    size_t hashOf(uint32_t key) const;
    void resize(size_t newCapacity);

public:
    HashTable();
    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    VEB* find(uint32_t key) const;
    void insert(uint32_t key, VEB* value);
    void remove(uint32_t key);
};

#endif
