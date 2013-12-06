#ifndef MURMURHASH1_INCLUDED
#  define MURMURHASH1_INCLUDED

#include "murmurhash.h"

VALUE murmur1_finish(VALUE self);
VALUE murmur1_to_i(VALUE self);
VALUE murmur1_s_rawdigest(int argc, VALUE *argv, VALUE klass);

#endif /* ifndef MURMURHASH1_INCLUDED */

