#include "murmurhash.h"
#include "murmurhash1.h"
#include "murmurhash2.h"
#include "murmurhash2a.h"

void
Init_murmurhash(void)
{
	VALUE mDigest, cDigest_StringBuffer;
	VALUE cDigest_MurmurHash1, cDigest_MurmurHash2, cDigest_MurmurHash2A;

	/* Digest::MurmurHash is require that Digest::StringBuffer */
	rb_require("digest/stringbuffer");
	mDigest = rb_path2class("Digest");
	cDigest_StringBuffer = rb_path2class("Digest::StringBuffer");

	/* class Digest::MurmurHash1 < Digest::StringBuffer */
	cDigest_MurmurHash1 = rb_define_class_under(mDigest, "MurmurHash1", cDigest_StringBuffer);
	rb_define_private_method(cDigest_MurmurHash1, "finish", murmur1_finish, 0);
	rb_define_method(cDigest_MurmurHash1, "to_i", murmur1_to_i, 0);
	rb_define_singleton_method(cDigest_MurmurHash1, "rawdigest", murmur1_s_rawdigest, -1);

	/* class Digest::MurmurHash2 < Digest::StringBuffer */
	cDigest_MurmurHash2 = rb_define_class_under(mDigest, "MurmurHash2", cDigest_StringBuffer);
	rb_define_private_method(cDigest_MurmurHash2, "finish", murmur2_finish, 0);
	rb_define_method(cDigest_MurmurHash2, "to_i", murmur2_to_i, 0);
	rb_define_singleton_method(cDigest_MurmurHash2, "rawdigest", murmur2_s_rawdigest, -1);

	/* class Digest::MurmurHash2A < Digest::StringBuffer */
	cDigest_MurmurHash2A = rb_define_class_under(mDigest, "MurmurHash2A", cDigest_StringBuffer);
	rb_define_private_method(cDigest_MurmurHash2A, "finish", murmur2a_finish, 0);
	rb_define_method(cDigest_MurmurHash2A, "to_i", murmur2a_to_i, 0);
	rb_define_singleton_method(cDigest_MurmurHash2A, "rawdigest", murmur2a_s_rawdigest, -1);
}
