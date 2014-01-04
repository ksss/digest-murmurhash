require 'mkmf'

$preload = %w[digest]
$CFLAGS << " -Wall"

create_makefile('digest/murmurhash/murmurhash')
