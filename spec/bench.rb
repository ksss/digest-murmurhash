#! /usr/bin/env ruby

lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

require 'digest/murmurhash'
require 'benchmark'

@rands = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789".split(//)
def rand_str length
  rand = "";
  length.times {
    rand << @rands.sample
  }
  rand
end

class Integer
  def to_32
    self & 0xffffffff
  end
end

def murmur_hash str, seed
  data = str.dup.unpack("C*")
  m = 0x5bd1e995
  r = 16
  length = str.bytesize
  h = (seed ^ (length * m).to_32).to_32

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

  h
end

n = 100000

a = Array.new(n, 0)
n.times do |i|
  a[i] = rand_str 20
end
seed = rand(2**32)
seed_str32 = [seed].pack("L")
seed_str64 = [seed].pack("Q")
c = Struct.new "Cases",
               :name,
               :func
cases = [
  c.new("pureRuby", proc{|x| murmur_hash x, seed }),
  c.new("MurmurHash1", proc{|x| Digest::MurmurHash1.digest x, seed_str32 }),
  c.new("MurmurHash2", proc{|x| Digest::MurmurHash2.digest x, seed_str32 }),
  c.new("MurmurHash2A", proc{|x| Digest::MurmurHash2A.digest x, seed_str32 }),
  c.new("MurmurHash64A", proc{|x| Digest::MurmurHash64A.digest x, seed_str64 }),
  c.new("MurmurHash64B", proc{|x| Digest::MurmurHash64B.digest x, seed_str64 }),
  c.new("MurmurHashNeutral2", proc{|x| Digest::MurmurHashNeutral2.digest x, seed_str32 }),
  c.new("MurmurHashAligned2", proc{|x| Digest::MurmurHashAligned2.digest x, seed_str32 }),
  c.new("MurmurHash3_x86_32", proc{|x| Digest::MurmurHash3_x86_32.digest x, seed_str32 }),
]
reals = {}
confrict = {}
confricts = {}

puts "### condition"
puts
puts "    RUBY_VERSION = #{RUBY_VERSION}"
puts "    count = #{n}"
puts
puts "### benchmark"
puts
puts "```"
Benchmark.bm do |x|
  cases.each do |c|
    i = 0
    z = x.report c.name do
      while i < n
        c.func.call(a[i])
        i += 1
      end
    end

    confrict.clear

    i = 0
    while i < n
      rethash = c.func.call(a[i])
      if confrict[rethash].nil?
        confrict[rethash] = 0
      else
        confrict[rethash] += 1
      end
      i += 1
    end
    reals[c.name] = z.real
    confricts[c.name] = confrict.count{|hash, count| 0 < count}
  end
end
puts "```"

puts
puts "### real second rate (pureRuby/)"
puts
reals.each do |name, real|
  puts "    " + (reals["pureRuby"] / real).to_s + "/" + name
end

puts
puts "### confrict count (/#{n})"
puts
confricts.each do |name, count|
  puts "    #{name}: #{count}"
end
