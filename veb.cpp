#include <iostream>
#include "veb.h"

VEB::VEB(int w) : w(w), min(-1), max(-1), summary(nullptr) {}

VEB::~VEB() {
    // TODO: liberar clusters e summary
}

void VEB::insert(uint64_t x) {
    // TODO
}

void VEB::remove(uint64_t x) {
    // TODO
}

int64_t VEB::successor(uint64_t x) const {
    return -1;  // TODO
}

int64_t VEB::predecessor(uint64_t x) const {
    return -1;  // TODO
}

void VEB::print() const {
    // TODO
}
