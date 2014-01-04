#include "murmurhash.h"
#include "murmurhash1.h"
#include "murmurhash2.h"
#include "murmurhash2a.h"
#include "murmurhash64a.h"

VALUE cDigest_MurmurHash1,
	  cDigest_MurmurHash2,
	  cDigest_MurmurHash2A,
	  cDigest_MurmurHash64A;
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
}
