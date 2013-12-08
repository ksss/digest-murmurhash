#! /usr/bin/env ruby

lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

require 'digest/stringbuffer'
require 'digest/murmurhash'
require 'benchmark'

class Prime37 < Digest::StringBuffer
  def initialize
    @prime = 37
  end

  def finish
    result = 0
    buffer.unpack("C*").each do |c|
      result += (c * @prime)
    end
    [result & 0xffffffff].pack("N")
  end
end

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

  h
end

@rands = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789".split(//)
def rand_str
  rand = "";
  20.times {
    rand << @rands[rand(62)]
  }
  rand
end

n = 100000
times_enum = n.times

a = Array.new(n, 0)
n.times do |i|
  a[i] = rand_str
end

c = Struct.new "Cases",
               :name,
               :func
cases = [
  c.new("pureRuby", proc{|x| murmur_hash x }),
  c.new("Prime37", proc{|x| Prime37.digest x }),
  c.new("MurmurHash1", proc{|x| Digest::MurmurHash1.rawdigest x }),
  c.new("MurmurHash2", proc{|x| Digest::MurmurHash2.rawdigest x }),
  c.new("MurmurHash2A", proc{|x| Digest::MurmurHash2A.rawdigest x }),
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
    z = x.report c.name do
      times_enum.each do |i|
        c.func.call(a[i])
      end
    end

    confrict.clear
    times_enum.each do |i|
      rethash = c.func.call(a[i])
      if confrict[rethash].nil?
        confrict[rethash] = 0
      else 
        confrict[rethash] += 1
      end
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
