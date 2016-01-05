/*
 * MurmurHash1 (C) Austin Appleby
 */

#include "init.h"

#define murmur1(r) do { \
  h *= m; \
  h ^= h >> r; \
} while(0)

static uint32_t
murmur_hash_process1(const char *data, uint32_t length, uint32_t seed)
{
  const uint32_t m = MURMURHASH_MAGIC;
  const uint8_t r = 16;
  uint32_t h;

  h = seed ^ (length * m);

  while (4 <= length) {
    h += *(uint32_t*)data;
    murmur1(r);
    data += 4;
    length -= 4;
  }

  switch (length) {
    case 3:
      h += data[2] << 16;
    case 2:
      h += data[1] << 8;
    case 1:
      h += data[0];
      murmur1(r);
  }

  murmur1(10);
  murmur1(17);

  return h;
}

VALUE
murmur1_finish(VALUE self)
{
  uint8_t digest[4];
  uint32_t h;

  h = _murmur_finish32(self, murmur_hash_process1);
  assign_by_endian_32(digest, h);
  return rb_str_new((const char*) digest, 4);
}

VALUE
murmur1_s_digest(int argc, VALUE *argv, VALUE klass)
{
  uint8_t digest[4];
  uint32_t h;
  h = _murmur_s_digest32(argc, argv, klass, murmur_hash_process1);
  assign_by_endian_32(digest, h);
  return rb_str_new((const char*) digest, 4);
}

VALUE
murmur1_s_rawdigest(int argc, VALUE *argv, VALUE klass)
{
  return UINT2NUM(_murmur_s_digest32(argc, argv, klass, murmur_hash_process1));
}
