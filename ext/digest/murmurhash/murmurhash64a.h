#ifndef MURMURHASH64A_INCLUDED
#  define MURMURHASH64A_INCLUDED

#include "murmurhash.h"

VALUE murmur64a_finish(VALUE self);
VALUE murmur64a_s_digest(int argc, VALUE *argv, VALUE klass);
VALUE murmur64a_s_rawdigest(int argc, VALUE *argv, VALUE klass);

#endif /* ifndef MURMURHASH64A_INCLUDED */
