/*
 * MurmurHash2 (C) Austin Appleby
 */

#include "murmurhash2.h"

static uint32_t
murmur_hash_process2(const char *data, uint32_t length, uint32_t seed)
{
	const uint32_t m = MURMURHASH_MAGIC;
	const uint8_t r = 24;
	uint32_t h, k;

	h = seed ^ length;

	while (4 <= length) {
		k = *(uint32_t*)data;
		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		length -= 4;
	}

	switch (length) {
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	}

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

static VALUE
_murmur2_finish(VALUE self)
{
	const char *seed = RSTRING_PTR(murmur_seed_get32(self));
	MURMURHASH(self, ptr);
	return murmur_hash_process2(ptr->buffer, ptr->p - ptr->buffer, *(uint32_t*)seed);
}

VALUE
murmur2_finish(VALUE self)
{
	uint8_t digest[4];
	uint64_t h;

	h = _murmur2_finish(self);
#if INTEGER_PACK_LITTLE_ENDIAN
	digest[3] = h >> 24;
	digest[2] = h >> 16;
	digest[1] = h >> 8;
	digest[0] = h;
#else
	digest[0] = h >> 24;
	digest[1] = h >> 16;
	digest[2] = h >> 8;
	digest[3] = h;
#endif
	return rb_str_new((const char*) digest, 4);
}

VALUE
murmur2_to_i(VALUE self)
{
	return ULONG2NUM(_murmur2_finish(self));
}

static uint32_t
_murmur2_s_digest(int argc, VALUE *argv, VALUE klass)
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

	return murmur_hash_process2(RSTRING_PTR(str), RSTRING_LEN(str), *(uint32_t*)seed);
}

VALUE
murmur2_s_digest(int argc, VALUE *argv, VALUE klass)
{
	uint8_t digest[4];
	uint32_t h;
	h = _murmur2_s_digest(argc, argv, klass);
#if INTEGER_PACK_LITTLE_ENDIAN
	digest[3] = h >> 24;
	digest[2] = h >> 16;
	digest[1] = h >> 8;
	digest[0] = h;
#else
	digest[0] = h >> 24;
	digest[1] = h >> 16;
	digest[2] = h >> 8;
	digest[3] = h;
#endif
	return rb_str_new((const char*) digest, 4);
}

VALUE
murmur2_s_hexdigest(int argc, VALUE *argv, VALUE klass)
{
	return hexencode_str_new(murmur2_s_digest(argc, argv, klass));
}

VALUE
murmur2_s_rawdigest(int argc, VALUE *argv, VALUE klass)
{
	return ULONG2NUM(_murmur2_s_digest(argc, argv, klass));
}

