/*
 * MurmurHashNeutral2 (C) Austin Appleby
 */

#include "init.h"

uint32_t
murmur_hash_process_neutral2(const char * key, uint32_t len, uint32_t seed)
{
  const uint32_t m = 0x5bd1e995;
  const int r = 24;

  uint32_t h = seed ^ len;

  const unsigned char * data = (const unsigned char *)key;

  while(len >= 4) {
    uint32_t k;

    k  = data[0];
    k |= data[1] << 8;
    k |= data[2] << 16;
    k |= data[3] << 24;

    k *= m;
    k ^= k >> r;
    k *= m;

    h *= m;
    h ^= k;

    data += 4;
    len -= 4;
  }

  switch(len) {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0];
            h *= m;
  };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

VALUE
murmur_neutral2_finish(VALUE self)
{
  uint8_t digest[4];
  uint32_t h;

  h = _murmur_finish32(self, murmur_hash_process_neutral2);
  assign_by_endian_32(digest, h);
  return rb_str_new((const char*) digest, 4);
}

VALUE
murmur_neutral2_s_digest(int argc, VALUE *argv, VALUE klass)
{
  uint8_t digest[4];
  uint32_t h;
  h = _murmur_s_digest32(argc, argv, klass, murmur_hash_process_neutral2);
  assign_by_endian_32(digest, h);
  return rb_str_new((const char*) digest, 4);
}

VALUE
murmur_neutral2_s_rawdigest(int argc, VALUE *argv, VALUE klass)
{
  return ULL2NUM(_murmur_s_digest32(argc, argv, klass, murmur_hash_process_neutral2));
}
