#ifndef MURMURHASH_INCLUDED
#  define MURMURHASH_INCLUDED

#include "ruby.h"

#if defined(_MSC_VER)
#  define BIG_CONSTANT(x) (x)
// Other compilers
#else   // defined(_MSC_VER)
#  define BIG_CONSTANT(x) (x##LLU)
#endif // !defined(_MSC_VER)

#define DEFAULT_SEED "\x00\x00\x00\x00\x00\x00\x00\x00"
#define MURMURHASH_MAGIC 0x5bd1e995
#define MURMURHASH_MAGIC64A BIG_CONSTANT(0xc6a4a7935bd1e995)
#if INTEGER_PACK_LITTLE_ENDIAN
#  define ASSINE_BY_ENDIAN_32(digest, h) do { \
	(digest)[3] = (h) >> 24; \
	(digest)[2] = (h) >> 16; \
	(digest)[1] = (h) >> 8; \
	(digest)[0] = (h); \
} while (0)
#  define ASSINE_BY_ENDIAN_64(digest, h) do { \
	(digest)[7] = (h) >> 56; \
	(digest)[6] = (h) >> 48; \
	(digest)[5] = (h) >> 40; \
	(digest)[4] = (h) >> 32; \
	(digest)[3] = (h) >> 24; \
	(digest)[2] = (h) >> 16; \
	(digest)[1] = (h) >> 8; \
	(digest)[0] = (h); \
} while (0)
#else
#  define ASSINE_BY_ENDIAN_32(digest, h) do { \
	(digest)[0] = (h) >> 24; \
	(digest)[1] = (h) >> 16; \
	(digest)[2] = (h) >> 8; \
	(digest)[3] = (h); \
} while (0)
#  define ASSINE_BY_ENDIAN_64(digest, h) do { \
	(digest)[0] = (h) >> 56; \
	(digest)[1] = (h) >> 48; \
	(digest)[2] = (h) >> 40; \
	(digest)[3] = (h) >> 32; \
	(digest)[4] = (h) >> 24; \
	(digest)[5] = (h) >> 16; \
	(digest)[6] = (h) >> 8; \
	(digest)[7] = (h); \
} while (0)
#endif

/* should be same type structure to digest/stringbuffer */
typedef struct {
	char* buffer;
	char* p;
	size_t memsize;
} buffer_t;

#define MURMURHASH(self, name) \
	buffer_t* name; \
	Data_Get_Struct(self, buffer_t, name); \
	if (name == NULL) { \
		rb_raise(rb_eArgError, "NULL found for " # name " when shouldn't be.'"); \
	}

VALUE murmur_seed_get32(VALUE self);
VALUE murmur_seed_get64(VALUE self);

extern ID id_seed, id_DEFAULT_SEED;

uint32_t _murmur_finish32(VALUE self, uint32_t (*process)(const char *, uint32_t, uint32_t));
uint64_t _murmur_finish64(VALUE self, uint64_t (*process)(const char *, uint32_t, uint64_t));
uint32_t _murmur_s_digest32(int argc, VALUE *argv, VALUE klass, uint32_t (*process)(const char *, uint32_t, uint32_t));
uint64_t _murmur_s_digest64(int argc, VALUE *argv, VALUE klass, uint64_t (*process)(const char *, uint32_t, uint64_t));
VALUE hexencode_str_new(VALUE str_digest);

#endif /* ifndef MURMURHASH_INCLUDED */

