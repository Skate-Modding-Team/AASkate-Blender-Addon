#pragma once

#include <limits.h>
#include <stdlib.h>
#include <iostream>

namespace
{

  // 32 bit magic FNV-1 prime
  const uint32_t rw_32_PRIME(0x01000193u);

  // 64 bit magic FNV-1 prime
  const uint64_t rw_64_PRIME(const uint64_t(0x100000001b3));

} // namespace

namespace rw
{

  const uint32_t RWHASH32_INIT(0x811c9dc5u);
  const uint64_t RWHASH64_INIT(const uint64_t(0xcbf29ce484222325));

  uint32_t
    RwHash32UInt32(uint32_t integer,
      uint32_t hashValue)
  {
    hashValue *= rw_32_PRIME;
    hashValue ^= (uint32_t)(integer & 0xff);
    integer >>= 8;
    hashValue *= rw_32_PRIME;
    hashValue ^= (uint32_t)(integer & 0xff);
    integer >>= 8;
    hashValue *= rw_32_PRIME;
    hashValue ^= (uint32_t)(integer & 0xff);
    integer >>= 8;
    hashValue *= rw_32_PRIME;
    hashValue ^= (uint32_t)(integer & 0xff);

    return hashValue;
  }

  uint32_t
    RwHash32Buffer(const uint8_t* buffer,
      size_t length,
      uint32_t hashValue)
  {
    const uint8_t* bufferEnd = buffer + length;        // beyond end of buffer

    // FNV-1 hash each byte in the buffer
    while (buffer < bufferEnd)
    {
      // multiply by the 32 bit FNV magic prime mod 2^32
      hashValue *= rw_32_PRIME;

      // xor the bottom with the current byte
      hashValue ^= (uint32_t)(*buffer++);
    }

    return hashValue;
  }

  uint32_t
    RwHash32String(const char* str,
      uint32_t hashValue)
  {
    const uint8_t* s = (const uint8_t*)str;    // unsigned string

    // FNV-1 hash each byte in the buffer
    while (*s)
    {
      // multiply by the 32 bit FNV magic prime mod 2^32
      hashValue *= rw_32_PRIME;

      // xor the bottom with the current byte
      hashValue ^= (uint32_t)(*s++);
    }

    return hashValue;
  }

  uint64_t
    RwHash64Buffer(const uint8_t* buffer,
      size_t length,
      uint64_t hashValue)
  {
    const uint8_t* bufferEnd = buffer + length;        // beyond end of buffer

    // FNV-1 hash each byte of the buffer
    while (buffer < bufferEnd)
    {
      // multiply by the 64 bit FNV magic prime mod 2^64
      hashValue *= rw_64_PRIME;

      // xor the bottom with the current byte
      hashValue ^= (uint64_t)(*buffer++);
    }

    return hashValue;
  }

  // This hash type is used for StreamFile Collections.
  uint64_t
    RwHash64String(const char* str,
      uint64_t hashValue)
  {
    const uint8_t* s = (const uint8_t*)str;    // unsigned string

    // FNV-1 hash each byte of the string
    while (*s)
    {
      // multiply by the 64 bit FNV magic prime mod 2^64
      hashValue *= rw_64_PRIME;

      // xor the bottom with the current byte
      hashValue ^= (uint64_t)(*s++);
    }

    return hashValue;
  }

  // Hashes used for everything except for the StreamFile Collections. (all big files)
  uint32_t djb2_hash_32(const char* str) {
    uint32_t hash = 5381;
    int c;

    while ((c = *str++)) {
      hash = ((hash << 5) + hash) + c;
    }

    return hash;
  }

  // Hashes used for everything except for the StreamFile Collections. (all big files)
  uint64_t djb2_hash_64(const char* str) {
    uint64_t hash = 5381;
    int c;

    while ((c = *str++)) {
      hash = ((hash << 5) + hash) + c;
    }

    return hash;
  }

} // namespace rw
