#ifndef MURMURHASH_INCLUDED
#  define MURMURHASH_INCLUDED

#include "ruby.h"

// Microsoft Visual Studio

#if defined(_MSC_VER)
  #define FORCE_INLINE    __forceinline
  #include <stdlib.h>
  #define ROTL32(x,y)     _rotl(x,y)
  #define ROTL64(x,y)     _rotl64(x,y)
  #define BIG_CONSTANT(x) (x)
#else   // defined(_MSC_VER)
  #define FORCE_INLINE inline __attribute__((always_inline))
  #define ROTL32(x,y)     rotl32(x,y)
  #define ROTL64(x,y)     rotl64(x,y)
  #define BIG_CONSTANT(x) (x##LLU)
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

void assign_by_endian_32(uint8_t *digest, uint32_t h);
void assign_by_endian_64(uint8_t *digest, uint64_t h);
void assign_by_endian_128(uint8_t*, void*);

uint32_t rotl32(uint32_t, int8_t);
uint64_t rotl64(uint64_t, int8_t);
uint32_t getblock32(const uint32_t*, int);
uint64_t getblock64(const uint64_t*, int);
uint32_t fmix32(uint32_t);
uint64_t fmix64(uint64_t);
uint32_t _murmur_finish32(VALUE, uint32_t (*)(const char*, uint32_t, uint32_t));
uint64_t _murmur_finish64(VALUE, uint64_t (*)(const char*, uint32_t, uint64_t));
void _murmur_finish128(VALUE, void*, void (*)(const char*, uint32_t, uint32_t, void*));
uint32_t _murmur_s_digest32(int, VALUE*, VALUE, uint32_t (*)(const char*, uint32_t, uint32_t));
uint64_t _murmur_s_digest64(int, VALUE*, VALUE, uint64_t (*)(const char*, uint32_t, uint64_t));
void _murmur_s_digest128(int, VALUE*, VALUE, void*, void (*)(const char*, uint32_t, uint32_t, void*));

VALUE murmur1_finish(VALUE);
VALUE murmur1_s_digest(int, VALUE*, VALUE);
VALUE murmur1_s_rawdigest(int, VALUE*, VALUE);
VALUE murmur2_finish(VALUE);
VALUE murmur2_s_digest(int, VALUE*, VALUE);
VALUE murmur2_s_rawdigest(int, VALUE*, VALUE);
VALUE murmur2a_finish(VALUE);
VALUE murmur2a_s_digest(int, VALUE*, VALUE);
VALUE murmur2a_s_rawdigest(int, VALUE*, VALUE);
VALUE murmur64a_finish(VALUE);
VALUE murmur64a_s_digest(int, VALUE*, VALUE);
VALUE murmur64a_s_rawdigest(int, VALUE*, VALUE);
VALUE murmur64b_finish(VALUE);
VALUE murmur64b_s_digest(int, VALUE*, VALUE);
VALUE murmur64b_s_rawdigest(int, VALUE*, VALUE);
VALUE murmur_neutral2_finish(VALUE);
VALUE murmur_neutral2_s_digest(int, VALUE*, VALUE);
VALUE murmur_neutral2_s_rawdigest(int, VALUE*, VALUE);
VALUE murmur_aligned2_finish(VALUE);
VALUE murmur_aligned2_s_digest(int, VALUE*, VALUE);
VALUE murmur_aligned2_s_rawdigest(int, VALUE*, VALUE);
VALUE murmur3_x86_32_finish(VALUE);
VALUE murmur3_x86_32_s_digest(int, VALUE*, VALUE);
VALUE murmur3_x86_32_s_rawdigest(int, VALUE*, VALUE);
VALUE murmur3_x86_128_finish(VALUE);
VALUE murmur3_x86_128_s_digest(int, VALUE*, VALUE);
VALUE murmur3_x86_128_s_rawdigest(int, VALUE*, VALUE);
VALUE murmur3_x64_128_finish(VALUE);
VALUE murmur3_x64_128_s_digest(int, VALUE*, VALUE);
VALUE murmur3_x64_128_s_rawdigest(int, VALUE*, VALUE);

#endif /* ifndef MURMURHASH_INCLUDED */
