#include "murmurhash.h"
#include "murmurhash1.h"
#include "murmurhash2.h"

void
Init_murmurhash(void)
{
	VALUE mDigest, cDigest_Base, cDigest_MurmurHash;
	VALUE cDigest_MurmurHash1, cDigest_MurmurHash2;

	/* Digest::MurmurHash is require that Digest module and Digest::Base class of CRuby built-in */
	rb_require("digest");
	mDigest = rb_path2class("Digest");
	cDigest_Base = rb_path2class("Digest::Base");

	/* class Digest::MurmurHash < Digest::Base */
	cDigest_MurmurHash = rb_define_class_under(mDigest, "MurmurHash", cDigest_Base);

	rb_define_alloc_func(cDigest_MurmurHash, murmur_alloc);

	/* instance methods (override on Digest::Base) */
	rb_define_method(cDigest_MurmurHash, "initialize_copy", murmur_initialize_copy, 1);
	rb_define_method(cDigest_MurmurHash, "reset", murmur_reset, 0);
	rb_define_method(cDigest_MurmurHash, "update", murmur_update, 1);
	rb_define_method(cDigest_MurmurHash, "<<", murmur_update, 1);
	rb_define_method(cDigest_MurmurHash, "digest_length", murmur_digest_length, 0);
	rb_define_method(cDigest_MurmurHash, "block_length", murmur_block_length, 0);

	rb_define_private_method(cDigest_MurmurHash, "finish", murmur1_finish, 0);
	rb_define_method(cDigest_MurmurHash, "to_i", murmur1_to_i, 0);
	rb_define_singleton_method(cDigest_MurmurHash, "rawdigest", murmur1_s_rawdigest, -1);

	/* class Digest::MurmurHash1 < Digest::MurmurHash */
	cDigest_MurmurHash1 = rb_define_class_under(mDigest, "MurmurHash1", cDigest_MurmurHash);
	rb_define_alloc_func(cDigest_MurmurHash1, murmur_alloc);

	/* class Digest::MurmurHash2 < Digest::MurmurHash */
	cDigest_MurmurHash2 = rb_define_class_under(mDigest, "MurmurHash2", cDigest_MurmurHash);
	rb_define_alloc_func(cDigest_MurmurHash2, murmur_alloc);

	/* instance methods (override on Digest::MurmurHash) */
	rb_define_private_method(cDigest_MurmurHash2, "finish", murmur2_finish, 0);
	rb_define_method(cDigest_MurmurHash2, "to_i", murmur2_to_i, 0);
	rb_define_singleton_method(cDigest_MurmurHash2, "rawdigest", murmur2_s_rawdigest, -1);
}
