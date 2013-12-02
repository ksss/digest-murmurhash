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
	VALUE data;
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
	ptr->data = rb_str_new("", 0);
}

static void
murmur_mark(murmur_t* ptr)
{
	rb_gc_mark(ptr->data);
}

static void
murmur_free(murmur_t* ptr)
{
	xfree(ptr);
}

static VALUE
murmur_alloc(VALUE self)
{
	murmur_t* ptr = ALLOC(murmur_t);
	murmur_init(ptr);
	return Data_Wrap_Struct(self, murmur_mark, murmur_free, ptr);
}

static VALUE
murmur_initialize_copy(VALUE self, VALUE obj)
{
	murmur_t *pctx1, *pctx2;

	if (self == obj) return self;

	rb_check_frozen(self);

	Data_Get_Struct(self, murmur_t, pctx1);
	Data_Get_Struct(obj, murmur_t, pctx2);
	memcpy(pctx1, pctx2, sizeof(murmur_t));

	return self;
}

static VALUE
murmur_reset(VALUE self)
{
	MURMURHASH(self, ptr);
	murmur_init(ptr);
	return self;
}

static VALUE
murmur_update(VALUE self, VALUE str)
{
	MURMURHASH(self, ptr);
	StringValue(str);
	rb_funcall(ptr->data, id_concat, 1, str);
	return self;
}

static uint32_t
murmur_hash_process(murmur_t* ptr)
{
	const uint32_t m = 0x5bd1e995;
	const uint8_t r = 16;
	uint32_t length, h;
	const char* p;

	p = RSTRING_PTR(ptr->data);
	length = RSTRING_LEN(ptr->data);
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
