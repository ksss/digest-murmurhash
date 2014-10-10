/*
 * MurmurHash3_x86_32 (C) Austin Appleby
 */

#include "init.h"

inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
  return (x << r) | (x >> (32 - r));
}
inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
  return (x << r) | (x >> (64 - r));
}

FORCE_INLINE uint32_t getblock32 ( const uint32_t * p, int i )
{
  return p[i];
}

FORCE_INLINE uint64_t getblock64 ( const uint64_t * p, int i )
{
  return p[i];
}

FORCE_INLINE uint32_t fmix32 ( uint32_t h )
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

FORCE_INLINE uint64_t fmix64 ( uint64_t k )
{
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xff51afd7ed558ccd);
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
  k ^= k >> 33;

  return k;
}

uint32_t
murmur_hash_process3_x86_32(const char * key, uint32_t len, uint32_t seed)
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 4;
  int i;

  uint32_t h1 = seed;

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  //----------
  // body

  const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

  for(i = -nblocks; i; i++)
  {
    uint32_t k1 = getblock32(blocks,i);

    k1 *= c1;
    k1 = ROTL32(k1,15);
    k1 *= c2;

    h1 ^= k1;
    h1 = ROTL32(h1,13);
    h1 = h1*5+0xe6546b64;
  }

  //----------
  // tail

  const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

  uint32_t k1 = 0;

  switch(len & 3)
  {
  case 3: k1 ^= tail[2] << 16;
  case 2: k1 ^= tail[1] << 8;
  case 1: k1 ^= tail[0];
          k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len;

  h1 = fmix32(h1);

  return h1;
}

VALUE
murmur3_x86_32_finish(VALUE self)
{
  uint8_t digest[4];
  uint32_t h;

  h = _murmur_finish32(self, murmur_hash_process3_x86_32);
  assign_by_endian_32(digest, h);
  return rb_str_new((const char*) digest, 4);
}

VALUE
murmur3_x86_32_s_digest(int argc, VALUE *argv, VALUE klass)
{
  uint8_t digest[4];
  uint64_t h;
  h = _murmur_s_digest32(argc, argv, klass, murmur_hash_process3_x86_32);
  assign_by_endian_32(digest, h);
  return rb_str_new((const char*) digest, 4);
}

VALUE
murmur3_x86_32_s_rawdigest(int argc, VALUE *argv, VALUE klass)
{
  return ULL2NUM(_murmur_s_digest32(argc, argv, klass, murmur_hash_process3_x86_32));
}
