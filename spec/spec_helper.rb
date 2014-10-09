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
    MurmurHashAligned2
  ]
end

def seed32
  (0..4).to_a.pack("C4")
end

def seed64
  (0..8).to_a.pack("C8")
end

