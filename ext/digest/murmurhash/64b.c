/*
 * MurmurHash64B (C) Austin Appleby
 */

#include "init.h"

uint64_t
murmur_hash_process64b(const char * key, uint32_t len, uint64_t seed)
{
  const uint32_t m = MURMURHASH_MAGIC;
  const int r = 24;

  uint32_t h1 = (uint32_t)(seed) ^ len;
  uint32_t h2 = (uint32_t)(seed >> 32);

  const uint32_t * data = (const uint32_t *)key;

  while(len >= 8) {
    uint32_t k1 = *data++;
    k1 *= m; k1 ^= k1 >> r; k1 *= m;
    h1 *= m; h1 ^= k1;
    len -= 4;

    uint32_t k2 = *data++;
    k2 *= m; k2 ^= k2 >> r; k2 *= m;
    h2 *= m; h2 ^= k2;
    len -= 4;
  }

  if(len >= 4) {
    uint32_t k1 = *data++;
    k1 *= m; k1 ^= k1 >> r; k1 *= m;
    h1 *= m; h1 ^= k1;
    len -= 4;
  }

  switch(len) {
    case 3: h2 ^= ((unsigned char*)data)[2] << 16;
    case 2: h2 ^= ((unsigned char*)data)[1] << 8;
    case 1: h2 ^= ((unsigned char*)data)[0];
            h2 *= m;
  };

  h1 ^= h2 >> 18; h1 *= m;
  h2 ^= h1 >> 22; h2 *= m;
  h1 ^= h2 >> 17; h1 *= m;
  h2 ^= h1 >> 19; h2 *= m;

  uint64_t h = (uint32_t)h1;

  h = (h << 32) | h2;

  return h;
}

VALUE
murmur64b_finish(VALUE self)
{
  uint8_t digest[8];
  uint64_t h;

  h = _murmur_finish64(self, murmur_hash_process64b);
  assign_by_endian_64(digest, h);
  return rb_str_new((const char*) digest, 8);
}

VALUE
murmur64b_s_digest(int argc, VALUE *argv, VALUE klass)
{
  uint8_t digest[8];
  uint64_t h;
  h = _murmur_s_digest64(argc, argv, klass, murmur_hash_process64b);
  assign_by_endian_64(digest, h);
  return rb_str_new((const char*) digest, 8);
}

VALUE
murmur64b_s_rawdigest(int argc, VALUE *argv, VALUE klass)
{
  return ULL2NUM(_murmur_s_digest64(argc, argv, klass, murmur_hash_process64b));
}
