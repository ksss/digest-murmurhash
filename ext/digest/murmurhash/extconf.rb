require 'mkmf'

have_header('ruby/digest.h')

$preload = %w[digest]

create_makefile('digest/murmurhash')
