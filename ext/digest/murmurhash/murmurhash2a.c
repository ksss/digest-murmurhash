/*
 * MurmurHash2A (C) Austin Appleby
 */

#include "murmurhash2a.h"

#define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }

static uint32_t
murmur_hash_process2a(const char *key, uint32_t length, uint32_t seed)
{
	const uint32_t m = MURMURHASH_MAGIC;
	const uint8_t r = 24;
	uint32_t h, k, t, l;
	const unsigned char *data = (const unsigned char *) key;

	l = length;
	h = seed;

	while (4 <= length) {
		k = *(uint32_t*)data;
		mmix(h,k);
		data += 4;
		length -= 4;
	}

	t = 0;
	switch (length) {
	case 3: t ^= data[2] << 16;
	case 2: t ^= data[1] << 8;
	case 1: t ^= data[0];
	}

	mmix(h,t);
	mmix(h,l);

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

static uint32_t
_murmur2a_finish(VALUE self)
{
	const char *seed = RSTRING_PTR(murmur_seed_get32(self));
	MURMURHASH(self, ptr);
	return murmur_hash_process2a(ptr->buffer, ptr->p - ptr->buffer, *(uint32_t*)seed);
}

VALUE
murmur2a_finish(VALUE self)
{
	uint8_t digest[4];
	uint64_t h;

	h = _murmur2a_finish(self);
	ASSINE_BY_ENDIAN_32(digest, h);
	return rb_str_new((const char*) digest, 4);
}

VALUE
murmur2a_to_i(VALUE self)
{
	return ULONG2NUM(_murmur2a_finish(self));
}

VALUE
murmur2a_s_digest(int argc, VALUE *argv, VALUE klass)
{
	uint8_t digest[4];
	uint32_t h;
	h = _murmur_s_digest32(argc, argv, klass, murmur_hash_process2a);
	ASSINE_BY_ENDIAN_32(digest, h);
	return rb_str_new((const char*) digest, 4);
}

VALUE
murmur2a_s_hexdigest(int argc, VALUE *argv, VALUE klass)
{
	return hexencode_str_new(murmur2a_s_digest(argc, argv, klass));
}

VALUE
murmur2a_s_rawdigest(int argc, VALUE *argv, VALUE klass)
{
	return ULL2NUM(_murmur_s_digest32(argc, argv, klass, murmur_hash_process2a));
}

