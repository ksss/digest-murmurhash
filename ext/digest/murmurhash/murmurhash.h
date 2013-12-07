#ifndef MURMURHASH_INCLUDED
#  define MURMURHASH_INCLUDED

#include "ruby.h"

#define MURMURHASH_MAGIC 0x5bd1e995

/* should be same type structure to digest/stringbuffer */
typedef struct {
	char* buffer;
	char* p;
	size_t memsize;
} buffer_t;

#define MURMURHASH(self, name) \
	buffer_t* name; \
	Data_Get_Struct(self, buffer_t, name); \
	if (name == NULL) { \
		rb_raise(rb_eArgError, "NULL found for " # name " when shouldn't be.'"); \
	}

#endif /* ifndef MURMURHASH_INCLUDED */

