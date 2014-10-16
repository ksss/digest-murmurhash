# Digest::MurmurHash

[![Build Status](https://travis-ci.org/ksss/digest-murmurhash.png?branch=master)](https://travis-ci.org/ksss/digest-murmurhash)

**MurmurHash** is a algorithm desiged by Austin Appleby.

This library have some classes for MurmurHash algorithm.

- **Digest::MurmurHash1** use algorithm MurmurHash1(32-bit).

- **Digest::MurmurHash2** use algorithm MurmurHash2(32-bit).

- **Digest::MurmurHash2A** use algorithm MurmurHash2A(32-bit).

- **Digest::MurmurHash64A** use algorithm MurmurHash64A(64-bit).

- **Digest::MurmurHash64B** use algorithm MurmurHash64B(64-bit).

- **Digest::MurmurHashNeutral2** use algorithm MurmurHashNeutral2(32-bit).

- **Digest::MurmurHashAligned2** use algorithm MurmurHashAligned2(32-bit).

- **Digest::MurmurHash3\_x86\_32** use algorithm MurmurHash3 for 32-bit platform (32-bit).

- **Digest::MurmurHash3\_x86\_128** use algorithm MurmurHash3 for 32-bit platform (128-bit).

- **Digest::MurmurHash3\_x64\_128** use algorithm MurmurHash3 for 64-bit platform (128-bit).

(**Digest::MurmurHash** class was nothing)

All classes compliance Digest API of Ruby.

## Usage

```ruby
require 'digest/murmurhash'

# MurmurHash1 can use like same than Digest::XXX.

p Digest::MurmurHash1.hexdigest('murmurhash') #=> d5ab09c7
p Digest::MurmurHash1.digest('murmurhash') #=> \xD5\xAB\x09\xC7
p Digest::MurmurHash1.rawdigest('murmurhash') #=> 3339299797
p Digest::MurmurHash1.file("./LICENSE.txt").hexdigest #=> "41962e71"

```

## Feature

- Very fast because hash algorithm is written by C extention
- You can try *all* Hurmurhash algorithms
- You can use same interface built-in Digest::{MD5,SH1,...} classes.

## Simple benchmark

```ruby
#! /usr/bin/env ruby

require 'benchmark'
require 'digest/md5'
require 'digest/sha1'
require 'digest/murmurhash'
require 'digest/siphash'

include Digest

n = 1000
str = "teststrings" * 1024 * 10

Benchmark.bm do |f|
  [MurmurHash1, MurmurHash2, MurmurHash2A, MurmurHash64A, MurmurHash64B, MurmurHashNeutral2, MurmurHashAligned2, MurmurHash3_x86_32, MurmurHash3_x86_128, MurmurHash3_x64_128, SipHash, MD5, SHA1].each do |klass|
    f.report(klass.to_s) {
      i = 0
      while i < n
        klass.digest(str)
        i += 1
      end
    }
  end
end
```

```
       user     system      total        real
Digest::MurmurHash1  0.050000   0.010000   0.060000 (  0.047889)
Digest::MurmurHash2  0.030000   0.000000   0.030000 (  0.034564)
Digest::MurmurHash2A  0.030000   0.010000   0.040000 (  0.031808)
Digest::MurmurHash64A  0.010000   0.000000   0.010000 (  0.018400)
Digest::MurmurHash64B  0.030000   0.000000   0.030000 (  0.027818)
Digest::MurmurHashNeutral2  0.040000   0.000000   0.040000 (  0.041021)
Digest::MurmurHashAligned2  0.020000   0.010000   0.030000 (  0.030409)
Digest::MurmurHash3_x86_32  0.130000   0.010000   0.140000 (  0.139622)
Digest::MurmurHash3_x86_128  0.120000   0.020000   0.140000 (  0.143768)
Digest::MurmurHash3_x64_128  0.070000   0.010000   0.080000 (  0.072687)
Digest::SipHash  0.060000   0.010000   0.070000 (  0.068243)
Digest::MD5  0.130000   0.010000   0.140000 (  0.153793)
Digest::SHA1  0.130000   0.020000   0.150000 (  0.137686)
```

## Installation

Add this line to your application's Gemfile:

    gem 'digest-murmurhash'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install digest-murmurhash

## License

See the file LICENSE.txt.

## See also

[https://sites.google.com/site/murmurhash/](https://sites.google.com/site/murmurhash/)
