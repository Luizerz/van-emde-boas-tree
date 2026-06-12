#include "hashtable.h"

HashTable::HashTable() : slots(nullptr), capacity(0), lg(0), count(0), used(0) {}

HashTable::~HashTable() {
    delete[] slots;
}

size_t HashTable::hashOf(uint32_t key) const {
    return 0;  // TODO
}

void HashTable::resize(size_t newCapacity) {
    // TODO
}

VEB* HashTable::find(uint32_t key) const {
    return nullptr;  // TODO
}

void HashTable::insert(uint32_t key, VEB* value) {
    // TODO
}

void HashTable::remove(uint32_t key) {
    // TODO
}
