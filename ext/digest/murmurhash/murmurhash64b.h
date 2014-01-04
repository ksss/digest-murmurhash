#ifndef MURMURHASH64B_INCLUDED
#  define MURMURHASH64B_INCLUDED

#include "murmurhash.h"

VALUE murmur64b_finish(VALUE self);
VALUE murmur64b_to_i(VALUE self);
VALUE murmur64b_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur64b_s_hexdigest(int argc, VALUE *argv, VALUE klass);
VALUE murmur64b_s_rawdigest(int argc, VALUE *argv, VALUE klass);

#endif /* ifndef MURMURHASH64B_INCLUDED */

