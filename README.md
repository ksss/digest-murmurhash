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

MurmurHash1	0.05145844700746238
MurmurHash2	0.03399810096016154
MurmurHash2A	0.032202123024035245
MurmurHash64A	0.01880742999492213
MurmurHash64B	0.02227930899243802
MurmurHashNeutral2	0.04134096298366785
MurmurHashAligned2	0.03514020598959178
MurmurHash3_x86_32	0.1174831380485557
MurmurHash3_x86_128	0.1244338889955543
MurmurHash3_x64_128	0.07122775202151388
Digest::MD5	0.1401238819817081
Digest::SHA1	0.09980463003739715
Digest::SHA256	0.2248609460075386
Digest::SHA2	0.2245309569989331
OpenSSL::HMAC(sha256)	0.23656713595846668
Base64	0.1765239799860865
Zlib.crc32	0.009485946036875248
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
