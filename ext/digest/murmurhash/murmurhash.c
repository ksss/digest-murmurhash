#include "murmurhash.h"
#include "murmurhash1.h"
#include "murmurhash2.h"
#include "murmurhash2a.h"
#include "murmurhash64a.h"
#include "murmurhash64b.h"
#include "murmurhash_neutral2.h"
#include "murmurhash_aligned2.h"

VALUE cDigest_MurmurHash1,
	  cDigest_MurmurHash2,
	  cDigest_MurmurHash2A,
	  cDigest_MurmurHash64A,
	  cDigest_MurmurHash64B,
	  cDigest_MurmurHashNeutral2,
	  cDigest_MurmurHashAligned2;
ID id_seed,
   id_DEFAULT_SEED;

VALUE
murmur_seed_get32(VALUE self)
{
	if (!rb_ivar_defined(self, id_seed)) {
		rb_ivar_set(self, id_seed, rb_usascii_str_new(DEFAULT_SEED, 4));
	}
	return rb_ivar_get(self, id_seed);
}

VALUE
murmur_seed_set32(VALUE self, VALUE obj)
{
	StringValue(obj);
	if (RSTRING_LEN(obj) != 4) {
		rb_raise(rb_eArgError, "seed string should 64 bit chars");
	}
	return rb_ivar_set(self, id_seed, obj);
}

VALUE
murmur_seed_get64(VALUE self)
{
	if (!rb_ivar_defined(self, id_seed)) {
		rb_ivar_set(self, id_seed, rb_usascii_str_new(DEFAULT_SEED, 8));
	}
	return rb_ivar_get(self, id_seed);
}

VALUE
murmur_seed_set64(VALUE self, VALUE obj)
{
	StringValue(obj);
	if (RSTRING_LEN(obj) != 8) {
		rb_raise(rb_eArgError, "seed string should 64 bit chars");
	}
	return rb_ivar_set(self, id_seed, obj);
}

uint32_t
_murmur_finish32(VALUE self, uint32_t (*process)(const char *, uint32_t, uint32_t))
{
	const char *seed = RSTRING_PTR(murmur_seed_get32(self));
	MURMURHASH(self, ptr);
	return process(ptr->buffer, ptr->p - ptr->buffer, *(uint32_t*)seed);
}

uint64_t
_murmur_finish64(VALUE self, uint64_t (*process)(const char *, uint32_t, uint64_t))
{
	const char *seed = RSTRING_PTR(murmur_seed_get64(self));
	MURMURHASH(self, ptr);
	return process(ptr->buffer, ptr->p - ptr->buffer, *(uint64_t*)seed);
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
			rb_raise(rb_eArgError, "seed string should 32 bit chars");
		}
		seed = RSTRING_PTR(argv[1]);
	} else {
		seed = RSTRING_PTR(rb_const_get(klass, id_DEFAULT_SEED));
	}

	return process(RSTRING_PTR(str), RSTRING_LEN(str), *(uint32_t*)seed);
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
			rb_raise(rb_eArgError, "seed string should 64 bit chars");
		}
		seed = RSTRING_PTR(argv[1]);
	} else {
		seed = RSTRING_PTR(rb_const_get(klass, id_DEFAULT_SEED));
	}

	return process(RSTRING_PTR(str), RSTRING_LEN(str), *(uint64_t*)seed);
}

/*
 * from https://github.com/ruby/ruby/blob/trunk/ext/digest/digest.c
 * Copyright (C) 1995-2001 Yukihiro Matsumoto
 * Copyright (C) 2001-2006 Akinori MUSHA
 */
VALUE
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

void
Init_murmurhash(void)
{
	VALUE mDigest, cDigest_StringBuffer;

	id_seed = rb_intern("seed");
	id_DEFAULT_SEED = rb_intern("DEFAULT_SEED");

	/* Digest::MurmurHash is require that Digest::StringBuffer */
	mDigest = rb_path2class("Digest");
	cDigest_StringBuffer = rb_path2class("Digest::StringBuffer");

	/* class Digest::MurmurHash1 < Digest::StringBuffer */
	cDigest_MurmurHash1 = rb_define_class_under(mDigest, "MurmurHash1", cDigest_StringBuffer);
	rb_define_const(cDigest_MurmurHash1, "DEFAULT_SEED", rb_usascii_str_new(DEFAULT_SEED, 4));
	rb_define_singleton_method(cDigest_MurmurHash1, "digest", murmur1_s_digest, -1);
	rb_define_singleton_method(cDigest_MurmurHash1, "hexdigest", murmur1_s_hexdigest, -1);
	rb_define_singleton_method(cDigest_MurmurHash1, "rawdigest", murmur1_s_rawdigest, -1);
	rb_define_private_method(cDigest_MurmurHash1, "finish", murmur1_finish, 0);
	rb_define_method(cDigest_MurmurHash1, "to_i", murmur1_to_i, 0);
	rb_define_method(cDigest_MurmurHash1, "seed", murmur_seed_get32, 0);
	rb_define_method(cDigest_MurmurHash1, "seed=", murmur_seed_set32, 1);

	/* class Digest::MurmurHash2 < Digest::StringBuffer */
	cDigest_MurmurHash2 = rb_define_class_under(mDigest, "MurmurHash2", cDigest_StringBuffer);
	rb_define_const(cDigest_MurmurHash2, "DEFAULT_SEED", rb_usascii_str_new(DEFAULT_SEED, 4));
	rb_define_singleton_method(cDigest_MurmurHash2, "digest", murmur2_s_digest, -1);
	rb_define_singleton_method(cDigest_MurmurHash2, "hexdigest", murmur2_s_hexdigest, -1);
	rb_define_singleton_method(cDigest_MurmurHash2, "rawdigest", murmur2_s_rawdigest, -1);
	rb_define_private_method(cDigest_MurmurHash2, "finish", murmur2_finish, 0);
	rb_define_method(cDigest_MurmurHash2, "to_i", murmur2_to_i, 0);
	rb_define_method(cDigest_MurmurHash2, "seed", murmur_seed_get32, 0);
	rb_define_method(cDigest_MurmurHash2, "seed=", murmur_seed_set32, 1);

	/* class Digest::MurmurHash2A < Digest::StringBuffer */
	cDigest_MurmurHash2A = rb_define_class_under(mDigest, "MurmurHash2A", cDigest_StringBuffer);
	rb_define_const(cDigest_MurmurHash2A, "DEFAULT_SEED", rb_usascii_str_new(DEFAULT_SEED, 4));
	rb_define_singleton_method(cDigest_MurmurHash2A, "digest", murmur2a_s_digest, -1);
	rb_define_singleton_method(cDigest_MurmurHash2A, "hexdigest", murmur2a_s_hexdigest, -1);
	rb_define_singleton_method(cDigest_MurmurHash2A, "rawdigest", murmur2a_s_rawdigest, -1);
	rb_define_private_method(cDigest_MurmurHash2A, "finish", murmur2a_finish, 0);
	rb_define_method(cDigest_MurmurHash2A, "to_i", murmur2a_to_i, 0);
	rb_define_method(cDigest_MurmurHash2A, "seed", murmur_seed_get32, 0);
	rb_define_method(cDigest_MurmurHash2A, "seed=", murmur_seed_set32, 1);

	/* class Digest::MurmurHash64A < Digest::StringBuffer */
	cDigest_MurmurHash64A = rb_define_class_under(mDigest, "MurmurHash64A", cDigest_StringBuffer);
	rb_define_const(cDigest_MurmurHash64A, "DEFAULT_SEED", rb_usascii_str_new(DEFAULT_SEED, 8));
	rb_define_singleton_method(cDigest_MurmurHash64A, "digest", murmur64a_s_digest, -1);
	rb_define_singleton_method(cDigest_MurmurHash64A, "hexdigest", murmur64a_s_hexdigest, -1);
	rb_define_singleton_method(cDigest_MurmurHash64A, "rawdigest", murmur64a_s_rawdigest, -1);
	rb_define_private_method(cDigest_MurmurHash64A, "finish", murmur64a_finish, 0);
	rb_define_method(cDigest_MurmurHash64A, "to_i", murmur64a_to_i, 0);
	rb_define_method(cDigest_MurmurHash64A, "seed", murmur_seed_get64, 0);
	rb_define_method(cDigest_MurmurHash64A, "seed=", murmur_seed_set64, 1);

	/* class Digest::MurmurHash64B < Digest::StringBuffer */
	cDigest_MurmurHash64B = rb_define_class_under(mDigest, "MurmurHash64B", cDigest_StringBuffer);
	rb_define_const(cDigest_MurmurHash64B, "DEFAULT_SEED", rb_usascii_str_new(DEFAULT_SEED, 8));
	rb_define_singleton_method(cDigest_MurmurHash64B, "digest", murmur64b_s_digest, -1);
	rb_define_singleton_method(cDigest_MurmurHash64B, "hexdigest", murmur64b_s_hexdigest, -1);
	rb_define_singleton_method(cDigest_MurmurHash64B, "rawdigest", murmur64b_s_rawdigest, -1);
	rb_define_private_method(cDigest_MurmurHash64B, "finish", murmur64b_finish, 0);
	rb_define_method(cDigest_MurmurHash64B, "to_i", murmur64b_to_i, 0);
	rb_define_method(cDigest_MurmurHash64B, "seed", murmur_seed_get64, 0);
	rb_define_method(cDigest_MurmurHash64B, "seed=", murmur_seed_set64, 1);

	/* class Digest::MurmurHashNeutral2 < Digest::StringBuffer */
	cDigest_MurmurHashNeutral2 = rb_define_class_under(mDigest, "MurmurHashNeutral2", cDigest_StringBuffer);
	rb_define_const(cDigest_MurmurHashNeutral2, "DEFAULT_SEED", rb_usascii_str_new(DEFAULT_SEED, 4));
	rb_define_singleton_method(cDigest_MurmurHashNeutral2, "digest", murmur_neutral2_s_digest, -1);
	rb_define_singleton_method(cDigest_MurmurHashNeutral2, "hexdigest", murmur_neutral2_s_hexdigest, -1);
	rb_define_singleton_method(cDigest_MurmurHashNeutral2, "rawdigest", murmur_neutral2_s_rawdigest, -1);
	rb_define_private_method(cDigest_MurmurHashNeutral2, "finish", murmur_neutral2_finish, 0);
	rb_define_method(cDigest_MurmurHashNeutral2, "to_i", murmur_neutral2_to_i, 0);
	rb_define_method(cDigest_MurmurHashNeutral2, "seed", murmur_seed_get32, 0);
	rb_define_method(cDigest_MurmurHashNeutral2, "seed=", murmur_seed_set32, 1);

	/* class Digest::MurmurHashAligned2 < Digest::StringBuffer */
	cDigest_MurmurHashAligned2 = rb_define_class_under(mDigest, "MurmurHashAligned2", cDigest_StringBuffer);
	rb_define_const(cDigest_MurmurHashAligned2, "DEFAULT_SEED", rb_usascii_str_new(DEFAULT_SEED, 4));
	rb_define_singleton_method(cDigest_MurmurHashAligned2, "digest", murmur_aligned2_s_digest, -1);
	rb_define_singleton_method(cDigest_MurmurHashAligned2, "hexdigest", murmur_aligned2_s_hexdigest, -1);
	rb_define_singleton_method(cDigest_MurmurHashAligned2, "rawdigest", murmur_aligned2_s_rawdigest, -1);
	rb_define_private_method(cDigest_MurmurHashAligned2, "finish", murmur_aligned2_finish, 0);
	rb_define_method(cDigest_MurmurHashAligned2, "to_i", murmur_aligned2_to_i, 0);
	rb_define_method(cDigest_MurmurHashAligned2, "seed", murmur_seed_get32, 0);
	rb_define_method(cDigest_MurmurHashAligned2, "seed=", murmur_seed_set32, 1);
}
