#ifndef MURMURHASH_INCLUDED
#  define MURMURHASH_INCLUDED

#include "ruby.h"
#ifdef HAVE_RUBY_DIGEST_H
#  include "ruby/digest.h"
#else
#  include "digest.h"
#endif

#define MURMURHASH_DIGEST_LENGTH 4
#define MURMURHASH_BLOCK_LENGTH 4
#define MURMURHASH_BUFFER_INIT 64
#define MURMURHASH_MAGIC 0x5bd1e995

typedef struct {
	char* buffer;
	char* p;
	size_t memsize;
} murmur_t;

#define MURMURHASH(self, name) \
	murmur_t* name; \
	Data_Get_Struct(self, murmur_t, name); \
	if (name == NULL) { \
		rb_raise(rb_eArgError, "NULL found for " # name " when shouldn't be.'"); \
	}

size_t murmur_buffer_length(murmur_t* ptr);
VALUE murmur_initialize_copy(VALUE copy, VALUE origin);
VALUE murmur_reset(VALUE self);
VALUE murmur_alloc(VALUE self);
VALUE murmur_update(VALUE self, VALUE str);
VALUE murmur_digest_length(VALUE self);
VALUE murmur_block_length(VALUE self);

#endif /* ifndef MURMURHASH_INCLUDED */

