#! /usr/bin/env ruby

lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

require 'digest/murmurhash'
require 'digest/md5'
require 'digest/sha1'
require 'openssl'
require 'zlib'
require 'benchmark'

@rands = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789".split(//)
def rand_str length
  rand = "";
  length.times {
    rand << @rands.sample
  }
  rand
end

seed = rand(2**32)
seed_str32 = [seed].pack("L")
seed_str64 = [seed].pack("Q")
c = Struct.new "Cases",
               :name,
               :func
cases = [
  c.new("MurmurHash1", proc{|x| Digest::MurmurHash1.digest x, seed_str32 }),
  c.new("MurmurHash2", proc{|x| Digest::MurmurHash2.digest x, seed_str32 }),
  c.new("MurmurHash2A", proc{|x| Digest::MurmurHash2A.digest x, seed_str32 }),
  c.new("MurmurHash64A", proc{|x| Digest::MurmurHash64A.digest x, seed_str64 }),
  c.new("MurmurHash64B", proc{|x| Digest::MurmurHash64B.digest x, seed_str64 }),
  c.new("MurmurHashNeutral2", proc{|x| Digest::MurmurHashNeutral2.digest x, seed_str32 }),
  c.new("MurmurHashAligned2", proc{|x| Digest::MurmurHashAligned2.digest x, seed_str32 }),
  c.new("MurmurHash3_x86_32", proc{|x| Digest::MurmurHash3_x86_32.digest x, seed_str32 }),
  c.new("MurmurHash3_x86_128", proc{|x| Digest::MurmurHash3_x86_128.digest x, seed_str32 }),
  c.new("MurmurHash3_x64_128", proc{|x| Digest::MurmurHash3_x64_128.digest x, seed_str32 }),
  c.new("Digest::MD5", proc{|x| Digest::MD5.digest x }),
  c.new("Digest::SHA1", proc{|x| Digest::SHA1.digest x }),
  c.new("Digest::SHA256", proc{|x| Digest::SHA256.digest x }),
  c.new("Digest::SHA2", proc{|x| Digest::SHA2.digest x }),
  c.new("OpenSSL::HMAC(sha256)", proc{|x| OpenSSL::HMAC.digest "sha256", seed_str32, x }),
  c.new("Base64", proc{|x| [x].pack("m0") }),
  c.new("Zlib.crc32", proc{|x| Zlib.crc32(x) }),
]

confrict = {}
confricts = {}

n = 100
a = Array.new(n, 0)
n.times do |i|
  a[i] = rand_str(1024*1024)
end

puts "### condition"
puts
puts "    RUBY_VERSION = #{RUBY_VERSION}"
puts "    count = #{n}"
puts "    data size = #{a[0].length / 1024} KB"
puts
puts "### benchmark"
puts
puts "```"

GC.start
cases.each do |c|
  i = 0
  b = Benchmark.realtime do
    while i < n
      c.func.call(a[i])
      i += 1
    end
  end
  puts "#{c.name}\t#{b}"

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
  confricts[c.name] = confrict.count{|hash, count| 0 < count}
end
puts "```"

puts
puts "### confrict count (/#{n})"
puts
confricts.each do |name, count|
  puts "    #{name}: #{count}"
end
