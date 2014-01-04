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

