#ifndef MURMURHASH2_INCLUDED
#  define MURMURHASH2_INCLUDED

#include "murmurhash.h"

VALUE murmur2_finish(VALUE self);
VALUE murmur2_to_i(VALUE self);
VALUE murmur2_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur2_s_hexdigest(int argc, VALUE *argv, VALUE klass);
VALUE murmur2_s_rawdigest(int argc, VALUE *argv, VALUE klass);

#endif /* ifndef MURMURHASH2_INCLUDED */

