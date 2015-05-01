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

## Benchmark

```
$ bundle ex ruby spec/bench.rb
### condition

    RUBY_VERSION = 2.2.2
    count = 100
    data size = 1024 KB

### benchmark

       user     system      total        real
MurmurHash1  0.060000   0.010000   0.070000 (  0.057101)
MurmurHash2  0.040000   0.000000   0.040000 (  0.037733)
MurmurHash2A  0.030000   0.000000   0.030000 (  0.032092)
MurmurHash64A  0.020000   0.000000   0.020000 (  0.015846)
MurmurHash64B  0.030000   0.000000   0.030000 (  0.025016)
MurmurHashNeutral2  0.040000   0.000000   0.040000 (  0.040506)
MurmurHashAligned2  0.030000   0.000000   0.030000 (  0.032861)
MurmurHash3_x86_32  0.110000   0.000000   0.110000 (  0.115907)
MurmurHash3_x86_128  0.120000   0.000000   0.120000 (  0.122998)
MurmurHash3_x64_128  0.070000   0.000000   0.070000 (  0.067370)
Digest::MD5  0.160000   0.000000   0.160000 (  0.159022)
Digest::SHA1  0.100000   0.000000   0.100000 (  0.100250)
Digest::SHA256  0.230000   0.000000   0.230000 (  0.224595)
Digest::SHA2  0.220000   0.000000   0.220000 (  0.228883)
OpenSSL::HMAC(sha256)  0.220000   0.000000   0.220000 (  0.227799)
Base64  0.130000   0.060000   0.190000 (  0.190484)

### confrict count (/100)

    MurmurHash1: 0
    MurmurHash2: 0
    MurmurHash2A: 0
    MurmurHash64A: 0
    MurmurHash64B: 0
    MurmurHashNeutral2: 0
    MurmurHashAligned2: 0
    MurmurHash3_x86_32: 0
    MurmurHash3_x86_128: 0
    MurmurHash3_x64_128: 0
    Digest::MD5: 0
    Digest::SHA1: 0
    Digest::SHA256: 0
    Digest::SHA2: 0
    OpenSSL::HMAC(sha256): 0
    Base64: 0
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
