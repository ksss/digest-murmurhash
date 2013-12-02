#! /usr/bin/env ruby

lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

require 'digest/murmurhash'
require 'benchmark'

class Integer
  def to_32
    self & 0xffffffff
  end
end

def murmur_hash str
  data = str.dup.unpack("C*")
  m = 0x5bd1e995
  r = 16
  length = str.bytesize
  h = (length * m).to_32

  while 4 <= length
    d = data.shift(4).pack("C*").unpack("I")[0]
    h = (h + d).to_32
    h = (h * m).to_32
    h ^= h >> r
    length -= 4
  end

  if 2 < length
    h = (h + (data[2] << 16).to_32).to_32
  end
  if 1 < length
    h = (h + (data[1] << 8).to_32).to_32
  end
  if 0 < length
    h = (h + data[0]).to_32
    h = (h * m).to_32
    h ^= h >> r
  end

  h = (h * m).to_32
  h ^= h >> 10
  h = (h * m).to_32
  h ^= h >> 17

  h.to_32
end

def rand_str
  rand = "";
  File.open("/dev/urandom").read(20).each_byte{|x| rand << sprintf("%02x",x)}
  rand
end

s = rand_str
p [murmur_hash(s)].pack("N")
p Digest::MurmurHash.digest(s)

Benchmark.bm do |x|
  n = 10000
  a = []
  n.times { |i|
   a[i] = rand_str
  }

  x.report {n.times{ |i|
    [murmur_hash(a[i])].pack("N")
  }}
  x.report {n.times{ |i|
    Digest::MurmurHash.digest(a[i])
  }}
end
