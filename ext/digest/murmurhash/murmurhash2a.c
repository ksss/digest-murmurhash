/*
 * MurmurHash2A (C) Austin Appleby
 */

#include "murmurhash2a.h"

#define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }

static uint32_t
murmur_hash_process2a(const void *key, uint32_t length)
{
	const uint32_t m = MURMURHASH_MAGIC;
	const uint8_t r = 24;
	uint32_t h, k, t, l;
	const unsigned char *data = (const unsigned char *) key;

	l = length;
	h = 0 ^ length;

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

VALUE
murmur2a_finish(VALUE self)
{
	uint32_t h;
	uint8_t digest[4];
	MURMURHASH(self, ptr);

	h = murmur_hash_process2a(ptr->buffer, ptr->p - ptr->buffer);

	digest[0] = h >> 24;
	digest[1] = h >> 16;
	digest[2] = h >> 8;
	digest[3] = h;

	return rb_str_new((const char*) digest, 4);
}

VALUE
murmur2a_to_i(VALUE self)
{
	MURMURHASH(self, ptr);
	return UINT2NUM(murmur_hash_process2a(ptr->buffer, ptr->p - ptr->buffer));
}

VALUE
murmur2a_s_digest(int argc, VALUE *argv, VALUE klass)
{
	VALUE str;
	uint32_t h;
	uint8_t digest[4];

	if (argc < 1)
		rb_raise(rb_eArgError, "no data given");

	str = *argv++;
	argc--;

	StringValue(str);

	h = murmur_hash_process2a(RSTRING_PTR(str), RSTRING_LEN(str));

	digest[0] = h >> 24;
	digest[1] = h >> 16;
	digest[2] = h >> 8;
	digest[3] = h;

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
	VALUE str;
	volatile VALUE obj;

	if (argc < 1)
		rb_raise(rb_eArgError, "no data given");

	str = *argv++;
	argc--;

	StringValue(str);

	return UINT2NUM(murmur_hash_process2a(RSTRING_PTR(str), RSTRING_LEN(str)));
}

