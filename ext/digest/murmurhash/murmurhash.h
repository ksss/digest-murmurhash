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

/*
 * from https://github.com/ruby/ruby/blob/trunk/ext/digest/digest.c
 * Copyright (C) 1995-2001 Yukihiro Matsumoto
 * Copyright (C) 2001-2006 Akinori MUSHA
 */
static VALUE
hexencode_str_new(VALUE str_digest)
{
	char *digest;
	size_t digest_len;
	size_t i;
	VALUE str;
	char *p;
	static const char hex[] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f'
	};

	StringValue(str_digest);
	digest = RSTRING_PTR(str_digest);
	digest_len = RSTRING_LEN(str_digest);

	if (LONG_MAX / 2 < digest_len) {
		rb_raise(rb_eRuntimeError, "digest string too long");
	}

	str = rb_usascii_str_new(0, digest_len * 2);

	for (i = 0, p = RSTRING_PTR(str); i < digest_len; i++) {
		unsigned char byte = digest[i];

		p[i + i]     = hex[byte >> 4];
		p[i + i + 1] = hex[byte & 0x0f];
	}

	return str;
}

#endif /* ifndef MURMURHASH_INCLUDED */

