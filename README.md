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
p Digest::MurmurHash1.file("./LICENSE.txt").hexdigest #=> "41962e71"

# Original method for performance
p Digest::MurmurHash1.rawdigest('murmurhash') #=> 3339299797
```

## Feature

- Very fast because hash algorithm is written by C extention
- You can try *all* Hurmurhash algorithms
- You can use same interface built-in Digest::{MD5,SH1,...} classes because each class inherited [Digest::Class](https://ruby-doc.org/stdlib-2.6.5/libdoc/digest/rdoc/Digest/Class.html).

## Benchmark

call `Digest::XXX.digest` (see also spec/bench.rb)

```
$ bundle exec ruby spec/bench.rb
### condition

    RUBY_VERSION = 2.6.5
    count = 1000
    data size = 100 KB

### benchmark

MurmurHash1	0.04397099930793047
MurmurHash2	0.030599999241530895
MurmurHash2A	0.03052499983459711
MurmurHash64A	0.014573999680578709
MurmurHash64B	0.027068999595940113
MurmurHashNeutral2	0.028548000380396843
MurmurHashAligned2	0.03340199962258339
MurmurHash3_x86_32	0.08531500026583672
MurmurHash3_x86_128	0.10534099955111742
MurmurHash3_x64_128	0.05008100066334009
Digest::MD5	0.1293069999665022
Digest::SHA1	0.10616999957710505
Digest::SHA256	0.23746399953961372
Digest::SHA2	0.23401100002229214
OpenSSL::HMAC(sha256)	0.23477800004184246
Base64	0.1534709995612502
Zlib.crc32	0.008456000126898289
```

![image](https://user-images.githubusercontent.com/935310/66634606-618de380-ec48-11e9-98ba-36f19152c951.png)


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
