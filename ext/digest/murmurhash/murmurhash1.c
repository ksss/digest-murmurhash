#include "murmurhash1.h"

static inline size_t
murmur1(uint32_t h, const uint8_t r)
{
	const uint32_t m = MURMURHASH_MAGIC;
	h *= m;
	h ^= h >> r;
	return h;
}

static uint32_t
murmur_hash_process1(murmur_t* ptr)
{
	const uint32_t m = MURMURHASH_MAGIC;
	const uint8_t r = 16;
	uint32_t length, h;
	const char* p;

	p = ptr->buffer;
	length = murmur_buffer_length(ptr);
	h = length * m;

	while (4 <= length) {
		h += *(uint32_t*)p;
		h = murmur1(h, r);
		p += 4;
		length -= 4;
	}

	switch (length) {
	case 3:
		h += p[2] << 16;
	case 2:
		h += p[1] << 8;
	case 1:
		h += p[0];
		h = murmur1(h, r);
	}

	h = murmur1(h, 10);
	h = murmur1(h, 17);

	return h;
}

VALUE
murmur1_finish(VALUE self)
{
	uint32_t h;
	uint8_t digest[4];
	MURMURHASH(self, ptr);

	h = murmur_hash_process1(ptr);

	digest[0] = h >> 24;
	digest[1] = h >> 16;
	digest[2] = h >> 8;
	digest[3] = h;

	return rb_str_new((const char*) digest, 4);
}

VALUE
murmur1_to_i(VALUE self)
{
	MURMURHASH(self, ptr);
	return UINT2NUM(murmur_hash_process1(ptr));
}

VALUE
murmur1_s_rawdigest(int argc, VALUE *argv, VALUE klass)
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
	return murmur1_to_i(obj);
}
