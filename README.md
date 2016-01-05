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

    RUBY_VERSION = 2.3.0
    count = 1000
    data size = 100 KB

### benchmark

MurmurHash1	0.16074114001821727
MurmurHash2	0.10484590596752241
MurmurHash2A	0.11195178102934733
MurmurHash64A	0.057314220000989735
MurmurHash64B	0.0911051009898074
MurmurHashNeutral2	0.164784726977814
MurmurHashAligned2	0.11094380798749626
MurmurHash3_x86_32	0.2993663040106185
MurmurHash3_x86_128	0.26751064899144694
MurmurHash3_x64_128	0.14950793702155352
Digest::MD5	0.1364029649994336
Digest::SHA1	0.105117603961844
Digest::SHA256	0.21450808201916516
Digest::SHA2	0.22369682899443433
OpenSSL::HMAC(sha256)	0.21367766003822908
Base64	0.17364867898868397
Zlib.crc32	0.009028148022480309
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
