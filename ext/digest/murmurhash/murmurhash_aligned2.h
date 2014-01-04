#ifndef MURMURHASH_ALIGNED2_INCLUDED
#  define MURMURHASH_ALIGNED2_INCLUDED

#include "murmurhash.h"

VALUE murmur_aligned2_finish(VALUE self);
VALUE murmur_aligned2_to_i(VALUE self);
VALUE murmur_aligned2_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur_aligned2_s_hexdigest(int argc, VALUE *argv, VALUE klass);
VALUE murmur_aligned2_s_rawdigest(int argc, VALUE *argv, VALUE klass);

#endif /* ifndef MURMURHASH_ALIGNED2_INCLUDED */

