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
murmur_hash_process2(murmur_t* ptr)
{
	const uint32_t m = MURMURHASH_MAGIC;
	const uint8_t r = 24;
	uint32_t length, h, k;
	const char* p;

	p = ptr->buffer;
	length = murmur_buffer_length(ptr);
	h = length * m;

	while (4 <= length) {
		k = *(uint32_t*)p;
		h = murmur2(h, k, r);
		p += 4;
		length -= 4;
	}

	switch (length) {
	case 3: h ^= p[2] << 16;
	case 2: h ^= p[1] << 8;
	case 1: h ^= p[0];
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

	h = murmur_hash_process2(ptr);

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
	return UINT2NUM(murmur_hash_process2(ptr));
}

VALUE
murmur2_s_rawdigest(int argc, VALUE *argv, VALUE klass)
{
	VALUE str;
	volatile VALUE obj;

	if (argc < 1)
		rb_raise(rb_eArgError, "no data given");

	str = *argv++;
	argc--;

	StringValue(str);

	obj = murmur_alloc(klass);

	murmur_update(obj, str);
	return murmur2_to_i(obj);
}

