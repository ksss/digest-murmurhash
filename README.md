# Digest::MurmurHash

[![Build Status](https://travis-ci.org/ksss/digest-murmurhash.png?branch=master)](https://travis-ci.org/ksss/digest-murmurhash)

**Digest::MurmurHash** is a class of use algorithm MurmurHash desiged by Austin Appleby.

**Digest::MurmurHash1** use algorithm MurmurHash1(32-bit).

**Digest::MurmurHash2** use algorithm MurmurHash2(32-bit).

All class compliance Digest API of Ruby.

## Usage

You can use same interface built in Digest::XXX classes.

```ruby

# MurmurHash and MurmurHash1 can use like same.

require 'digest/murmurhash'

p Digest::MurmurHash.hexdigest('murmurhash') #=> 'c709abd5'
p Digest::MurmurHash.file("./LICENSE.txt").hexdigest #=> '712e9641'

p Digest::MurmurHash1.hexdigest('murmurhash') #=> 'c709abd5'
p Digest::MurmurHash1.file("./LICENSE.txt").hexdigest #=> '712e9641'

# and MurmurHash2 too. but return another value because using another algorithm.

p Digest::MurmurHash2.hexdigest('murmurhash') #=> '33f67c7e'
p Digest::MurmurHash2.file("./LICENSE.txt").hexdigest #=> '78678326'
```

## Class tree

**Digest::MurmurHash** < Digest::Base

**Digest::MurmurHash1** < Digest::MurmurHash

**Digest::MurmurHash2** < Digest::MurmurHash

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
