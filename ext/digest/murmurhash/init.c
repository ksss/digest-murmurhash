#include "init.h"

ID id_DEFAULT_SEED;
ID iv_seed;
ID iv_buffer;


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

void
assign_by_endian_32(uint8_t *digest, uint32_t h)
{
  if (BIGENDIAN_P()) {
    digest[0] = h >> 24;
    digest[1] = h >> 16;
    digest[2] = h >> 8;
    digest[3] = h;
  }
  else {
    digest[3] = h >> 24;
    digest[2] = h >> 16;
    digest[1] = h >> 8;
    digest[0] = h;
  }
}

void
assign_by_endian_64(uint8_t *digest, uint64_t h)
{
  if (BIGENDIAN_P()) {
    digest[0] = h >> 56;
    digest[1] = h >> 48;
    digest[2] = h >> 40;
    digest[3] = h >> 32;
    digest[4] = h >> 24;
    digest[5] = h >> 16;
    digest[6] = h >> 8;
    digest[7] = h;
  }
  else {
    digest[7] = h >> 56;
    digest[6] = h >> 48;
    digest[5] = h >> 40;
    digest[4] = h >> 32;
    digest[3] = h >> 24;
    digest[2] = h >> 16;
    digest[1] = h >> 8;
    digest[0] = h;
  }
}

void
assign_by_endian_128(uint8_t *digest, void *out)
{
  int i;

  if (BIGENDIAN_P()) {
    for (i = 0; i < 4; i++) {
      digest[(i*4)  ] = ((uint32_t*)out)[i] >> 24;
      digest[(i*4)+1] = ((uint32_t*)out)[i] >> 16;
      digest[(i*4)+2] = ((uint32_t*)out)[i] >> 8;
      digest[(i*4)+3] = ((uint32_t*)out)[i];
    }
  }
  else {
    for (i = 0; i < 4; i++) {
      digest[16-(i*4)-1] = ((uint32_t*)out)[i] >> 24;
      digest[16-(i*4)-2] = ((uint32_t*)out)[i] >> 16;
      digest[16-(i*4)-3] = ((uint32_t*)out)[i] >> 8;
      digest[16-(i*4)-4] = ((uint32_t*)out)[i];
    }
  }
}

static uint32_t
rstring2uint32_t(VALUE str)
{
  long len = RSTRING_LEN(str);
  if (UINT32_MAX < len) {
    rb_raise(rb_eRangeError, "String length=%ld will overflow from long to uint32_t", len);
  }
  return (uint32_t)len;
}

uint32_t
_murmur_finish32(VALUE self, uint32_t (*process)(const char*, uint32_t, uint32_t))
{
  const char *seed = RSTRING_PTR(rb_ivar_get(self, iv_seed));
  VALUE buffer = rb_ivar_get(self, iv_buffer);
  return process(RSTRING_PTR(buffer), rstring2uint32_t(buffer), *(uint32_t*)seed);
}

uint64_t
_murmur_finish64(VALUE self, uint64_t (*process)(const char*, uint32_t, uint64_t))
{
  const char *seed = RSTRING_PTR(rb_ivar_get(self, iv_seed));
  VALUE buffer = rb_ivar_get(self, iv_buffer);
  return process(RSTRING_PTR(buffer), rstring2uint32_t(buffer), *(uint64_t*)seed);
}

void
_murmur_finish128(VALUE self, void *out, void (*process)(const char*, uint32_t, uint32_t, void*))
{
  const char *seed = RSTRING_PTR(rb_ivar_get(self, iv_seed));
  VALUE buffer = rb_ivar_get(self, iv_buffer);
  process(RSTRING_PTR(buffer), rstring2uint32_t(buffer), *(uint32_t*)seed, out);
}

uint32_t
_murmur_s_digest32(int argc, VALUE *argv, VALUE klass, uint32_t (*process)(const char *, uint32_t, uint32_t))
{
  VALUE str;
  const char *seed;

  if (argc < 1)
    rb_raise(rb_eArgError, "no data given");

  str = *argv;

  StringValue(str);

  if (1 < argc) {
    StringValue(argv[1]);
    if (RSTRING_LEN(argv[1]) != 4) {
      rb_raise(rb_eArgError, "seed string should be 4 length");
    }
    seed = RSTRING_PTR(argv[1]);
  } else {
    seed = RSTRING_PTR(rb_const_get(klass, id_DEFAULT_SEED));
  }

  return process(RSTRING_PTR(str), rstring2uint32_t(str), *(uint32_t*)seed);
}

uint64_t
_murmur_s_digest64(int argc, VALUE *argv, VALUE klass, uint64_t (*process)(const char *, uint32_t, uint64_t))
{
  VALUE str;
  const char *seed;

  if (argc < 1)
    rb_raise(rb_eArgError, "no data given");

  str = *argv;

  StringValue(str);

  if (1 < argc) {
    StringValue(argv[1]);
    if (RSTRING_LEN(argv[1]) != 8) {
      rb_raise(rb_eArgError, "seed string should be 8 length");
    }
    seed = RSTRING_PTR(argv[1]);
  } else {
    seed = RSTRING_PTR(rb_const_get(klass, id_DEFAULT_SEED));
  }

  return process(RSTRING_PTR(str), rstring2uint32_t(str), *(uint64_t*)seed);
}

void
_murmur_s_digest128(int argc, VALUE *argv, VALUE klass, void *out, void (*process)(const char *, uint32_t, uint32_t, void *))
{
  VALUE str;
  const char *seed;
  int seed_length = 4;

  if (argc < 1)
    rb_raise(rb_eArgError, "no data given");

  str = *argv;

  StringValue(str);

  if (1 < argc) {
    StringValue(argv[1]);
    if (RSTRING_LEN(argv[1]) != seed_length) {
      rb_raise(rb_eArgError, "seed string should be %d length", seed_length);
    }
    seed = RSTRING_PTR(argv[1]);
  } else {
    seed = RSTRING_PTR(rb_const_get(klass, id_DEFAULT_SEED));
  }

  process(RSTRING_PTR(str), rstring2uint32_t(str), *(uint32_t*)seed, out);
}


void
Init_ext(void)
{
  VALUE cDigest_MurmurHash1,
        cDigest_MurmurHash2,
        cDigest_MurmurHash2A,
        cDigest_MurmurHash64A,
        cDigest_MurmurHash64B,
        cDigest_MurmurHashNeutral2,
        cDigest_MurmurHashAligned2,
        cDigest_MurmurHash3_x86_32,
        cDigest_MurmurHash3_x86_128,
        cDigest_MurmurHash3_x64_128;

  id_DEFAULT_SEED = rb_intern("DEFAULT_SEED");
  iv_seed = rb_intern("@seed");
  iv_buffer = rb_intern("@buffer");

  cDigest_MurmurHash1 = rb_path2class("Digest::MurmurHash1");
  rb_define_singleton_method(cDigest_MurmurHash1, "digest", murmur1_s_digest, -1);
  rb_define_singleton_method(cDigest_MurmurHash1, "rawdigest", murmur1_s_rawdigest, -1);
  rb_define_private_method(cDigest_MurmurHash1, "finish", murmur1_finish, 0);

  cDigest_MurmurHash2 = rb_path2class("Digest::MurmurHash2");
  rb_define_singleton_method(cDigest_MurmurHash2, "digest", murmur2_s_digest, -1);
  rb_define_singleton_method(cDigest_MurmurHash2, "rawdigest", murmur2_s_rawdigest, -1);
  rb_define_private_method(cDigest_MurmurHash2, "finish", murmur2_finish, 0);

  cDigest_MurmurHash2A = rb_path2class("Digest::MurmurHash2A");
  rb_define_singleton_method(cDigest_MurmurHash2A, "digest", murmur2a_s_digest, -1);
  rb_define_singleton_method(cDigest_MurmurHash2A, "rawdigest", murmur2a_s_rawdigest, -1);
  rb_define_private_method(cDigest_MurmurHash2A, "finish", murmur2a_finish, 0);

  cDigest_MurmurHash64A = rb_path2class("Digest::MurmurHash64A");
  rb_define_singleton_method(cDigest_MurmurHash64A, "digest", murmur64a_s_digest, -1);
  rb_define_singleton_method(cDigest_MurmurHash64A, "rawdigest", murmur64a_s_rawdigest, -1);
  rb_define_private_method(cDigest_MurmurHash64A, "finish", murmur64a_finish, 0);

  cDigest_MurmurHash64B = rb_path2class("Digest::MurmurHash64B");
  rb_define_singleton_method(cDigest_MurmurHash64B, "digest", murmur64b_s_digest, -1);
  rb_define_singleton_method(cDigest_MurmurHash64B, "rawdigest", murmur64b_s_rawdigest, -1);
  rb_define_private_method(cDigest_MurmurHash64B, "finish", murmur64b_finish, 0);

  cDigest_MurmurHashNeutral2 = rb_path2class("Digest::MurmurHashNeutral2");
  rb_define_singleton_method(cDigest_MurmurHashNeutral2, "digest", murmur_neutral2_s_digest, -1);
  rb_define_singleton_method(cDigest_MurmurHashNeutral2, "rawdigest", murmur_neutral2_s_rawdigest, -1);
  rb_define_private_method(cDigest_MurmurHashNeutral2, "finish", murmur_neutral2_finish, 0);

  cDigest_MurmurHashAligned2 = rb_path2class("Digest::MurmurHashAligned2");
  rb_define_singleton_method(cDigest_MurmurHashAligned2, "digest", murmur_aligned2_s_digest, -1);
  rb_define_singleton_method(cDigest_MurmurHashAligned2, "rawdigest", murmur_aligned2_s_rawdigest, -1);
  rb_define_private_method(cDigest_MurmurHashAligned2, "finish", murmur_aligned2_finish, 0);

  cDigest_MurmurHash3_x86_32 = rb_path2class("Digest::MurmurHash3_x86_32");
  rb_define_singleton_method(cDigest_MurmurHash3_x86_32, "digest", murmur3_x86_32_s_digest, -1);
  rb_define_singleton_method(cDigest_MurmurHash3_x86_32, "rawdigest", murmur3_x86_32_s_rawdigest, -1);
  rb_define_private_method(cDigest_MurmurHash3_x86_32, "finish", murmur3_x86_32_finish, 0);

  cDigest_MurmurHash3_x86_128 = rb_path2class("Digest::MurmurHash3_x86_128");
  rb_define_singleton_method(cDigest_MurmurHash3_x86_128, "digest", murmur3_x86_128_s_digest, -1);
  rb_define_singleton_method(cDigest_MurmurHash3_x86_128, "rawdigest", murmur3_x86_128_s_rawdigest, -1);
  rb_define_private_method(cDigest_MurmurHash3_x86_128, "finish", murmur3_x86_128_finish, 0);

  cDigest_MurmurHash3_x64_128 = rb_path2class("Digest::MurmurHash3_x64_128");
  rb_define_singleton_method(cDigest_MurmurHash3_x64_128, "digest", murmur3_x64_128_s_digest, -1);
  rb_define_singleton_method(cDigest_MurmurHash3_x64_128, "rawdigest", murmur3_x64_128_s_rawdigest, -1);
  rb_define_private_method(cDigest_MurmurHash3_x64_128, "finish", murmur3_x64_128_finish, 0);
}
