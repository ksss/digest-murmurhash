/*
 * MurmurHash64A (C) Austin Appleby
 */

#include "murmurhash64a.h"

static uint64_t
murmur_hash_process64a(const char *key, size_t len, uint64_t seed)
{
	const uint64_t m = MURMURHASH_MAGIC64A;
	const int r = 47;

	uint64_t h = seed ^ (len * m);

	const uint64_t * data = (const uint64_t *)key;
	const uint64_t * end = data + (len/8);

	while(data != end)
	{
		uint64_t k = *data++;

		k *= m; 
		k ^= k >> r; 
		k *= m; 

		h ^= k;
		h *= m; 
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch(len & 7) {
	case 7: h ^= (uint64_t)((uint64_t)data2[6] << (uint64_t)48);
	case 6: h ^= (uint64_t)((uint64_t)data2[5] << (uint64_t)40);
	case 5: h ^= (uint64_t)((uint64_t)data2[4] << (uint64_t)32);
	case 4: h ^= (uint64_t)((uint64_t)data2[3] << (uint64_t)24);
	case 3: h ^= (uint64_t)((uint64_t)data2[2] << (uint64_t)16);
	case 2: h ^= (uint64_t)((uint64_t)data2[1] << (uint64_t)8 );
	case 1: h ^= (uint64_t)((uint64_t)data2[0]                );
			h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}

static uint64_t
_murmur64a_finish(VALUE self)
{
	const char *seed = RSTRING_PTR(murmur_seed_get64(self));
	MURMURHASH(self, ptr);
	return murmur_hash_process64a(ptr->buffer, ptr->p - ptr->buffer, *(uint64_t*)seed);
}

VALUE
murmur64a_finish(VALUE self)
{
	uint8_t digest[8];
	uint64_t h;

	h = _murmur64a_finish(self);
#if INTEGER_PACK_LITTLE_ENDIAN
	digest[7] = h >> 56;
	digest[6] = h >> 48;
	digest[5] = h >> 40;
	digest[4] = h >> 32;
	digest[3] = h >> 24;
	digest[2] = h >> 16;
	digest[1] = h >> 8;
	digest[0] = h;
#else
	digest[0] = h >> 56;
	digest[1] = h >> 48;
	digest[2] = h >> 40;
	digest[3] = h >> 32;
	digest[4] = h >> 24;
	digest[5] = h >> 16;
	digest[6] = h >> 8;
	digest[7] = h;
#endif
	return rb_str_new((const char*) digest, 8);
}

VALUE
murmur64a_to_i(VALUE self)
{
	return ULL2NUM(_murmur64a_finish(self));
}

static uint64_t
_murmur64a_s_digest(int argc, VALUE *argv, VALUE klass)
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

	return murmur_hash_process64a(RSTRING_PTR(str), RSTRING_LEN(str), *(uint64_t*)seed);
}

VALUE
murmur64a_s_digest(int argc, VALUE *argv, VALUE klass)
{
	uint8_t digest[8];
	uint64_t h;
	h = _murmur64a_s_digest(argc, argv, klass);
#if INTEGER_PACK_LITTLE_ENDIAN
	digest[7] = h >> 56;
	digest[6] = h >> 48;
	digest[5] = h >> 40;
	digest[4] = h >> 32;
	digest[3] = h >> 24;
	digest[2] = h >> 16;
	digest[1] = h >> 8;
	digest[0] = h;
#else
	digest[0] = h >> 56;
	digest[1] = h >> 48;
	digest[2] = h >> 40;
	digest[3] = h >> 32;
	digest[4] = h >> 24;
	digest[5] = h >> 16;
	digest[6] = h >> 8;
	digest[7] = h;
#endif
	return rb_str_new((const char*) digest, 8);
}

VALUE
murmur64a_s_hexdigest(int argc, VALUE *argv, VALUE klass)
{
	return hexencode_str_new(murmur64a_s_digest(argc, argv, klass));
}

VALUE
murmur64a_s_rawdigest(int argc, VALUE *argv, VALUE klass)
{
	return ULL2NUM(_murmur64a_s_digest(argc, argv, klass));
}
