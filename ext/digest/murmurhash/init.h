#ifndef MURMURHASH_INCLUDED
#  define MURMURHASH_INCLUDED

#include "ruby.h"

#if defined(_MSC_VER)
#  define BIG_CONSTANT(x) (x)
// Other compilers
#else   // defined(_MSC_VER)
#  define BIG_CONSTANT(x) (x##LLU)
#endif // !defined(_MSC_VER)

#ifdef DYNAMIC_ENDIAN
  /* for universal binary of NEXTSTEP and MacOS X */
  /* useless since autoconf 2.63? */
  static int
  is_bigendian(void)
  {
    static int init = 0;
    static int endian_value;
    char *p;

    if (init) return endian_value;
    init = 1;
    p = (char*)&init;
    return endian_value = p[0] ? 0 : 1;
  }
#  define BIGENDIAN_P() (is_bigendian())
#elif defined(WORDS_BIGENDIAN)
#  define BIGENDIAN_P() 1
#else
#  define BIGENDIAN_P() 0
#endif

#define MURMURHASH_MAGIC 0x5bd1e995
#define MURMURHASH_MAGIC64A BIG_CONSTANT(0xc6a4a7935bd1e995)

void assign_by_endian_32(uint8_t *digest, uint64_t h);
void assign_by_endian_64(uint8_t *digest, uint64_t h);

uint32_t _murmur_finish32(VALUE self, uint32_t (*process)(const char *, uint32_t, uint32_t));
uint64_t _murmur_finish64(VALUE self, uint64_t (*process)(const char *, uint32_t, uint64_t));
uint32_t _murmur_s_digest32(int argc, VALUE *argv, VALUE klass, uint32_t (*process)(const char *, uint32_t, uint32_t));
uint64_t _murmur_s_digest64(int argc, VALUE *argv, VALUE klass, uint64_t (*process)(const char *, uint32_t, uint64_t));

VALUE murmur1_finish(VALUE self);
VALUE murmur1_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur1_s_rawdigest(int argc, VALUE *argv, VALUE klass);
VALUE murmur2_finish(VALUE self);
VALUE murmur2_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur2_s_rawdigest(int argc, VALUE *argv, VALUE klass);
VALUE murmur2a_finish(VALUE self);
VALUE murmur2a_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur2a_s_rawdigest(int argc, VALUE *argv, VALUE klass);
VALUE murmur64a_finish(VALUE self);
VALUE murmur64a_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur64a_s_rawdigest(int argc, VALUE *argv, VALUE klass);
VALUE murmur64b_finish(VALUE self);
VALUE murmur64b_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur64b_s_rawdigest(int argc, VALUE *argv, VALUE klass);
VALUE murmur_aligned2_finish(VALUE self);
VALUE murmur_aligned2_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur_aligned2_s_rawdigest(int argc, VALUE *argv, VALUE klass);
VALUE murmur_neutral2_finish(VALUE self);
VALUE murmur_neutral2_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur_neutral2_s_rawdigest(int argc, VALUE *argv, VALUE klass);

#endif /* ifndef MURMURHASH_INCLUDED */
