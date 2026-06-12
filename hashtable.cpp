#include "hashtable.h"

static const size_t INITIAL_CAPACITY = 4;

HashTable::HashTable() : slots(nullptr), capacity(0), lg(0), count(0), used(0) {}

HashTable::~HashTable() {
    delete[] slots;
}

size_t HashTable::hashOf(uint32_t key) const {
    // Hashing multiplicativo: pega os lg bits mais significativos do
    // produto pela constante de Fibonacci (2^32 / razão áurea).
    uint32_t prod = key * 2654435769u;
    return prod >> (32 - lg);
}

void HashTable::resize(size_t newCapacity) {
    Slot* old = slots;
    size_t oldCapacity = capacity;

    capacity = newCapacity;
    lg = 0;
    while (((size_t)1 << lg) < capacity) lg++;

    slots = new Slot[capacity];
    for (size_t i = 0; i < capacity; i++)
        slots[i].state = EMPTY;

    if (old != nullptr) {
        for (size_t i = 0; i < oldCapacity; i++) {
            if (old[i].state != OCCUPIED) continue;
            size_t j = hashOf(old[i].key);
            while (slots[j].state == OCCUPIED)
                j = (j + 1) & (capacity - 1);
            slots[j].key = old[i].key;
            slots[j].value = old[i].value;
            slots[j].state = OCCUPIED;
        }
        delete[] old;
    }
    used = count;  // a reconstrução elimina os slots DELETED
}

VEB* HashTable::find(uint32_t key) const {
    if (capacity == 0) return nullptr;
    size_t i = hashOf(key);
    while (slots[i].state != EMPTY) {
        if (slots[i].state == OCCUPIED && slots[i].key == key)
            return slots[i].value;
        i = (i + 1) & (capacity - 1);
    }
    return nullptr;
}

void HashTable::insert(uint32_t key, VEB* value) {
    if (capacity == 0) {
        resize(INITIAL_CAPACITY);
    }
    else if (2 * (used + 1) > capacity) {
        // Table doubling quando a ocupação real passa de 1/2; se o excesso
        // for só de slots DELETED, reconstrói com a mesma capacidade.
        size_t newCapacity = (2 * (count + 1) > capacity) ? capacity * 2 : capacity;
        resize(newCapacity);
    }

    size_t i = hashOf(key);
    size_t target = (size_t)-1;
    while (slots[i].state != EMPTY) {
        if (slots[i].state == OCCUPIED && slots[i].key == key) {
            slots[i].value = value;
            return;
        }
        if (slots[i].state == DELETED && target == (size_t)-1)
            target = i;  // reaproveita o primeiro slot removido do caminho
        i = (i + 1) & (capacity - 1);
    }
    if (target == (size_t)-1) {
        target = i;
        used++;
    }
    slots[target].key = key;
    slots[target].value = value;
    slots[target].state = OCCUPIED;
    count++;
}

void HashTable::remove(uint32_t key) {
    if (capacity == 0) return;
    size_t i = hashOf(key);
    while (slots[i].state != EMPTY) {
        if (slots[i].state == OCCUPIED && slots[i].key == key) {
            slots[i].state = DELETED;
            count--;
            // Table halving quando a ocupação cai a 1/8.
            if (capacity > INITIAL_CAPACITY && 8 * count <= capacity)
                resize(capacity / 2);
            return;
        }
        i = (i + 1) & (capacity - 1);
    }
}
