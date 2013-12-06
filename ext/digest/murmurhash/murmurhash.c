#include "murmurhash.h"

static void
murmur_init(murmur_t* ptr)
{
	ptr->buffer = (char*) xmalloc(sizeof(char) * MURMURHASH_BUFFER_INIT);
	ptr->p = ptr->buffer;
	ptr->memsize = MURMURHASH_BUFFER_INIT;
}

static void
murmur_realloc(murmur_t* ptr, size_t size)
{
	char* buffer;

	buffer = (char*) xrealloc(ptr->buffer, size);
	if (ptr->buffer == NULL)
		rb_raise(rb_eNoMemError, "failed to realloc");

	ptr->buffer = buffer;
}

inline static void
murmur_free(murmur_t* ptr)
{
	xfree(ptr->buffer);
}

inline size_t
murmur_buffer_length(murmur_t* ptr)
{
	return ptr->p - ptr->buffer;
}

VALUE
murmur_alloc(VALUE self)
{
	murmur_t* ptr = ALLOC(murmur_t);
	murmur_init(ptr);
	return Data_Wrap_Struct(self, 0, murmur_free, ptr);
}

VALUE
murmur_initialize_copy(VALUE copy, VALUE origin)
{
	murmur_t *ptr_copy, *ptr_origin;
	size_t buffer_len;

	if (copy == origin) return copy;

	rb_check_frozen(copy);

	Data_Get_Struct(copy, murmur_t, ptr_copy);
	Data_Get_Struct(origin, murmur_t, ptr_origin);

	buffer_len = murmur_buffer_length(ptr_origin);

	if (ptr_copy->memsize < ptr_origin->memsize) {
		murmur_realloc(ptr_copy, sizeof(char) * ptr_origin->memsize);
		ptr_copy->memsize = ptr_origin->memsize;
	}

	memcpy(ptr_copy->buffer, ptr_origin->buffer, buffer_len);
	ptr_copy->p = ptr_copy->buffer + buffer_len;

	return copy;
}

VALUE
murmur_reset(VALUE self)
{
	MURMURHASH(self, ptr);
	ptr->p = ptr->buffer;
	return self;
}

VALUE
murmur_update(VALUE self, VALUE str)
{
	size_t buffer_len, str_len, require, newsize;
	const char* str_p;
	MURMURHASH(self, ptr);

	StringValue(str);
	str_p = RSTRING_PTR(str);
	str_len = RSTRING_LEN(str);
	buffer_len = murmur_buffer_length(ptr);
	require = buffer_len + str_len;
	if (ptr->memsize < require) {
		newsize = ptr->memsize;
		while (newsize < require) {
			newsize *= 2;
		}
		murmur_realloc(ptr, sizeof(char) * newsize);
		ptr->p = ptr->buffer + buffer_len;
		ptr->memsize = newsize;
	}
	memcpy(ptr->p, str_p, str_len);
	ptr->p += str_len;

	return self;
}

VALUE
murmur_digest_length(VALUE self)
{
	return INT2FIX(MURMURHASH_DIGEST_LENGTH);
}

VALUE
murmur_block_length(VALUE self)
{
	return INT2FIX(MURMURHASH_BLOCK_LENGTH);
}

