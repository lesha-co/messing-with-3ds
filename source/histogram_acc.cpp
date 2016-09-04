//
// Created by lichevsky on 04.09.16.
//

#include "histogram_acc.h"

histogram_acc::histogram_acc() {
    store = map<size_t ,size_t >();
}

void histogram_acc::put(size_t value) {
    store[value]++;
}

map<size_t, size_t> histogram_acc::get() {
    return store;
}
