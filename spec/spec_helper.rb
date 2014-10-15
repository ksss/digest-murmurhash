require 'digest/murmurhash'
include Digest

def all_classes
  [
    MurmurHash1,
    MurmurHash2,
    MurmurHash2A,
    MurmurHash64A,
    MurmurHash64B,
    MurmurHashNeutral2,
    MurmurHashAligned2,
    MurmurHash3_x86_32,
    MurmurHash3_x86_128,
    MurmurHash3_x64_128
  ]
end

def seed_str(klass)
  (0..klass::DEFAULT_SEED.length).to_a.pack("C#{klass::DEFAULT_SEED.length}")
end
