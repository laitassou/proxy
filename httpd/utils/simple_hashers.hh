
#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#define XXH_STATIC_LINKING_ONLY
#include <xxhash.h>
#pragma GCC diagnostic pop

#include "hashing.hh"

template<typename H>
concept SimpleHasher = HasherReturning<H, size_t>;

struct simple_xx_hasher : public hasher {
    XXH64_state_t _state;
    simple_xx_hasher(uint64_t seed = 0) noexcept {
        XXH64_reset(&_state, seed);
    }
    void update(const char* ptr, size_t length) noexcept override {
        XXH64_update(&_state, ptr, length);
    }
    size_t finalize() {
        return static_cast<size_t>(XXH64_digest(&_state));
    }
};