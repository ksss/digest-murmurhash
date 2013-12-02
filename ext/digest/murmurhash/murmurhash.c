#include "ruby.h"
#ifdef HAVE_RUBY_DIGEST_H
  #include "ruby/digest.h"
#else
  #include "digest.h"
#endif

#define MURMURHASH_DIGEST_LENGTH 4
#define MURMURHASH_BLOCK_LENGTH 4


ID id_concat;

typedef struct {
	char* data;
	char* p;
	size_t memsize;
} murmur_t;

#define MURMURHASH(self, name) \
	murmur_t* name; \
	Data_Get_Struct(self, murmur_t, name); \
	if (name == NULL) { \
		rb_raise(rb_eArgError, "NULL found for " # name " when shouldn't be.'"); \
	}

static void
murmur_init(murmur_t* ptr)
{
	ptr->data = (char*) malloc(sizeof(char) * 64);
	ptr->p = ptr->data;
	ptr->memsize = 64;
}

static void
murmur_mark(murmur_t* ptr)
{
}

static void
murmur_free(murmur_t* ptr)
{
	free(ptr->data);
}

static VALUE
murmur_alloc(VALUE self)
{
	murmur_t* ptr = ALLOC(murmur_t);
	murmur_init(ptr);
	return Data_Wrap_Struct(self, murmur_mark, murmur_free, ptr);
}

static VALUE
murmur_initialize_copy(VALUE copy, VALUE origin)
{
	murmur_t *ptr_copy, *ptr_origin;
	size_t data_len;

	if (copy == origin) return copy;

	rb_check_frozen(copy);

	Data_Get_Struct(copy, murmur_t, ptr_copy);
	Data_Get_Struct(origin, murmur_t, ptr_origin);

	data_len = ptr_origin->p - ptr_origin->data;
	ptr_copy->data = (char*) malloc(sizeof(char) * ptr_origin->memsize);
	memcpy(ptr_copy->data, ptr_origin->data, data_len);
	ptr_copy->p = ptr_copy->data + data_len;
	ptr_copy->memsize = ptr_origin->memsize;

	return copy;
}

static VALUE
murmur_reset(VALUE self)
{
	MURMURHASH(self, ptr);
	ptr->p = ptr->data;
	return self;
}

static VALUE
murmur_update(VALUE self, VALUE str)
{
	size_t data_len, str_len, require, newsize;
	const char* str_p;
	MURMURHASH(self, ptr);

	StringValue(str);
	str_p = RSTRING_PTR(str);
	str_len = RSTRING_LEN(str);
	data_len = (ptr->p - ptr->data);
	require = data_len + str_len;
	if (ptr->memsize < require) {
		newsize = ptr->memsize;
		while (newsize < require) {
			newsize *= 2;
		}
		ptr->data = realloc(ptr->data, sizeof(char) * newsize);
		ptr->p = ptr->data + data_len;
		ptr->memsize = newsize;
	}
	memcpy(ptr->p, str_p, str_len);
	ptr->p += str_len;

	return self;
}

static uint32_t
murmur_hash_process(murmur_t* ptr)
{
	const uint32_t m = 0x5bd1e995;
	const uint8_t r = 16;
	uint32_t length, h;
	const char* p;

	p = ptr->data;
	length = ptr->p - ptr->data;
	h = length * m;

	while (4 <= length) {
		h += *(uint32_t*)p;
		h *= m;
		h ^= h >> r;
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
			h *= m;
			h ^= h >> r;
	}

	h *= m;
	h ^= h >> 10;
	h *= m;
	h ^= h >> 17;

	return h;
}

static VALUE
murmur_finish(VALUE self)
{
	uint32_t h;
	uint8_t digest[MURMURHASH_DIGEST_LENGTH];
	MURMURHASH(self, ptr);

	h = murmur_hash_process(ptr);

	digest[0] = (h >> 24);
	digest[1] = (h >> 16);
	digest[2] = (h >> 8);
	digest[3] = (h);

	return rb_str_new((const char*) digest, 4);
}

static VALUE
murmur_digest_length(VALUE self)
{
	return INT2NUM(MURMURHASH_DIGEST_LENGTH);
}

static VALUE
murmur_block_length(VALUE self)
{
	return INT2NUM(MURMURHASH_BLOCK_LENGTH);
}

static VALUE
murmur_to_i(VALUE self)
{
	MURMURHASH(self, ptr);
	return UINT2NUM(murmur_hash_process(ptr));
}

void
Init_murmurhash()
{
	VALUE mDigest, cDigest_Base, cDigest_MurmurHash;

	id_concat = rb_intern("concat");

	rb_require("digest");

	mDigest = rb_path2class("Digest");
	cDigest_Base = rb_path2class("Digest::Base");

	cDigest_MurmurHash = rb_define_class_under(mDigest, "MurmurHash", cDigest_Base);

	rb_define_alloc_func(cDigest_MurmurHash, murmur_alloc);
	rb_define_method(cDigest_MurmurHash, "initialize_copy", murmur_initialize_copy, 1);
	rb_define_method(cDigest_MurmurHash, "reset", murmur_reset, 0);
	rb_define_method(cDigest_MurmurHash, "update", murmur_update, 1);
	rb_define_private_method(cDigest_MurmurHash, "finish", murmur_finish, 0);
	rb_define_method(cDigest_MurmurHash, "digest_length", murmur_digest_length, 0);
	rb_define_method(cDigest_MurmurHash, "block_length", murmur_block_length, 0);

	rb_define_method(cDigest_MurmurHash, "to_i", murmur_to_i, 0);
}
