# Digest::MurmurHash1 and Digest::MurmurHash2

[![Build Status](https://travis-ci.org/ksss/digest-murmurhash.png?branch=master)](https://travis-ci.org/ksss/digest-murmurhash)

Digest::MurmurHash1 is a class of use algorithm MurmurHash1(32-bit) desiged by Austin Appleby.
Digest::MurmurHash2 is a class of use algorithm MurmurHash2(32-bit) desiged by Austin Appleby.

Digest::MurmurHash1 and Digest::MurmurHash2 compliance Digest API of Ruby.

## Usage

All interface compliance Digest::Base.

You can use same interface built in Digest::XXX classes.

```ruby
require 'digest/murmurhash'

p Digest::MurmurHash.hexdigest('murmurhash') #=> 'c709abd5'
p Digest::MurmurHash.file("./LICENSE.txt").hexdigest #=> '712e9641'

# MurmurHash and MurmurHash1 can use like same.

p Digest::MurmurHash1.hexdigest('murmurhash') #=> 'c709abd5'
p Digest::MurmurHash1.file("./LICENSE.txt").hexdigest #=> '712e9641'

# and MurmurHash2 to. but use another algorithm.

p Digest::MurmurHash2.hexdigest('murmurhash') #=> '33f67c7e'
p Digest::MurmurHash2.file("./LICENSE.txt").hexdigest #=> '78678326'
```

## Class tree

Digest::MurmurHash < Digest::Base
Digest::MurmurHash1 < Digest::MurmurHash
Digest::MurmurHash2 < Digest::MurmurHash

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
