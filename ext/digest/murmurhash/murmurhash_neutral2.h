#ifndef MURMURHASH_NEUTRAL2_INCLUDED
#  define MURMURHASH_NEUTRAL2_INCLUDED

#include "murmurhash.h"

VALUE murmur_neutral2_finish(VALUE self);
VALUE murmur_neutral2_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur_neutral2_s_rawdigest(int argc, VALUE *argv, VALUE klass);

#endif /* ifndef MURMURHASH_NEUTRAL2_INCLUDED */

