/*
 * MurmurHash2 (C) Austin Appleby
 */

#include "murmurhash2.h"

static inline size_t
murmur2(uint32_t h, uint32_t k, const uint8_t r)
{
	const uint32_t m = MURMURHASH_MAGIC;
	k *= m;
	k ^= k >> r;
	k *= m;

	h *= m;
	h ^= k;
	return h;
}

static uint32_t
murmur_hash_process2(const char *data, uint32_t length)
{
	const uint32_t m = MURMURHASH_MAGIC;
	const uint8_t r = 24;
	uint32_t h, k;

	h = length * m;

	while (4 <= length) {
		k = *(uint32_t*)data;
		h = murmur2(h, k, r);
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

VALUE
murmur2_finish(VALUE self)
{
	uint32_t h;
	uint8_t digest[4];
	MURMURHASH(self, ptr);

	h = murmur_hash_process2(ptr->buffer, ptr->p - ptr->buffer);

	digest[0] = h >> 24;
	digest[1] = h >> 16;
	digest[2] = h >> 8;
	digest[3] = h;

	return rb_str_new((const char*) digest, 4);
}

VALUE
murmur2_to_i(VALUE self)
{
	MURMURHASH(self, ptr);
	return UINT2NUM(murmur_hash_process2(ptr->buffer, ptr->p - ptr->buffer));
}

VALUE
murmur2_s_digest(int argc, VALUE *argv, VALUE klass)
{
	VALUE str;
	uint32_t h;
	uint8_t digest[4];

	if (argc < 1)
		rb_raise(rb_eArgError, "no data given");

	str = *argv++;
	argc--;

	StringValue(str);

	h = murmur_hash_process2(RSTRING_PTR(str), RSTRING_LEN(str));

	digest[0] = h >> 24;
	digest[1] = h >> 16;
	digest[2] = h >> 8;
	digest[3] = h;

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
	VALUE str;

	if (argc < 1)
		rb_raise(rb_eArgError, "no data given");

	str = *argv++;
	argc--;

	StringValue(str);

	return UINT2NUM(murmur_hash_process2(RSTRING_PTR(str), RSTRING_LEN(str)));
}

