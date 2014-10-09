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
ID id_DEFAULT_SEED;
ID iv_seed;
ID iv_buffer;

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

void
assign_by_endian_32(uint8_t *digest, uint64_t h)
{
	if (is_bigendian()) {
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
	if (is_bigendian()) {
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

uint32_t
_murmur_finish32(VALUE self, uint32_t (*process)(const char *, uint32_t, uint32_t))
{
	const char *seed = RSTRING_PTR(rb_ivar_get(self, iv_seed));
	VALUE buffer = rb_ivar_get(self, iv_buffer);
	return process(RSTRING_PTR(buffer), RSTRING_LEN(buffer), *(uint32_t*)seed);
}

uint64_t
_murmur_finish64(VALUE self, uint64_t (*process)(const char *, uint32_t, uint64_t))
{
	const char *seed = RSTRING_PTR(rb_ivar_get(self, iv_seed));
	VALUE buffer = rb_ivar_get(self, iv_buffer);
	return process(RSTRING_PTR(buffer), RSTRING_LEN(buffer), *(uint64_t*)seed);
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

void
Init_murmurhash(void)
{
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
}
