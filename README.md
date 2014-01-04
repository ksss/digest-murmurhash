# Digest::MurmurHash

[![Build Status](https://travis-ci.org/ksss/digest-murmurhash.png?branch=master)](https://travis-ci.org/ksss/digest-murmurhash)

**MurmurHash** is a algorithm desiged by Austin Appleby.

**Digest::MurmurHash1** use algorithm MurmurHash1(32-bit).

**Digest::MurmurHash2** use algorithm MurmurHash2(32-bit).

**Digest::MurmurHash2A** use algorithm MurmurHash2A(32-bit).

**Digest::MurmurHash64A** use algorithm MurmurHash64A(64-bit).

**Digest::MurmurHash64B** use algorithm MurmurHash64B(64-bit).

**Digest::MurmurHashNeutral2** use algorithm MurmurHashNeutral2(32-bit).

**Digest::MurmurHashAligned2** use algorithm MurmurHashAligned2(32-bit).

(**Digest::MurmurHash** exist to write only version number)

All classes compliance Digest API of Ruby.

## Usage

You can use same interface built-in Digest::XXX classes.

```ruby
require 'digest/murmurhash'

# MurmurHash1 can use like same than Digest::XXX.

p Digest::MurmurHash1.hexdigest('murmurhash') #=> 'c709abd5'
p Digest::MurmurHash1.file("./LICENSE.txt").hexdigest #=> '712e9641'

# and MurmurHash2 too. but return another value because using another algorithm.

p Digest::MurmurHash2.hexdigest('murmurhash') #=> '33f67c7e'
p Digest::MurmurHash2.file("./LICENSE.txt").hexdigest #=> '78678326'
```

## Class tree

**Digest::MurmurHash1** < Digest::StringBuffer

**Digest::MurmurHash2** < Digest::StringBuffer

**Digest::MurmurHash2A** < Digest::StringBuffer

**Digest::MurmurHash64A** < Digest::StringBuffer

**Digest::MurmurHash64B** < Digest::StringBuffer

**Digest::MurmurHashNeutral2** < Digest::StringBuffer

**Digest::MurmurHashAligned2** < Digest::StringBuffer

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
