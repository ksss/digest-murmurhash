/*
 * MurmurHash64A (C) Austin Appleby
 */

#include "init.h"

static uint64_t
murmur_hash_process64a(const char *key, uint32_t len, uint64_t seed)
{
  const uint64_t m = MURMURHASH_MAGIC64A;
  const int r = 47;

  uint64_t h = seed ^ (len * m);

  const uint64_t * data = (const uint64_t *)key;
  const uint64_t * end = data + (len/8);

  while(data != end)
  {
    uint64_t k = *data++;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
  }

  const unsigned char * data2 = (const unsigned char*)data;

  switch(len & 7) {
    case 7: h ^= (uint64_t)((uint64_t)data2[6] << (uint64_t)48);
    case 6: h ^= (uint64_t)((uint64_t)data2[5] << (uint64_t)40);
    case 5: h ^= (uint64_t)((uint64_t)data2[4] << (uint64_t)32);
    case 4: h ^= (uint64_t)((uint64_t)data2[3] << (uint64_t)24);
    case 3: h ^= (uint64_t)((uint64_t)data2[2] << (uint64_t)16);
    case 2: h ^= (uint64_t)((uint64_t)data2[1] << (uint64_t)8 );
    case 1: h ^= (uint64_t)((uint64_t)data2[0]                );
            h *= m;
  };

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}

VALUE
murmur64a_finish(VALUE self)
{
  uint8_t digest[8];
  uint64_t h;

  h = _murmur_finish64(self, murmur_hash_process64a);
  assign_by_endian_64(digest, h);
  return rb_str_new((const char*) digest, 8);
}

VALUE
murmur64a_s_digest(int argc, VALUE *argv, VALUE klass)
{
  uint8_t digest[8];
  uint64_t h;
  h = _murmur_s_digest64(argc, argv, klass, murmur_hash_process64a);
  assign_by_endian_64(digest, h);
  return rb_str_new((const char*) digest, 8);
}

VALUE
murmur64a_s_rawdigest(int argc, VALUE *argv, VALUE klass)
{
  return ULL2NUM(_murmur_s_digest64(argc, argv, klass, murmur_hash_process64a));
}
