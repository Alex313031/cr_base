#ifndef BASE_HASH_SUPERFASTHASH_H_
#define BASE_HASH_SUPERFASTHASH_H_

#include <cstdint>

#include "base/compiler_specific.h"

uint32_t SuperFastHash (const char* data, int len);

void SuperFastHashSeeded(const void* key, int len, uint32_t /*seed*/, void* out);

#endif
