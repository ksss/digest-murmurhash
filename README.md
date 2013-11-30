# Digest::MurmurHash

Digest::MurmurHash is a class use algorithm of MurmurHash(MurmurHash1 32-bit) desiged by Austin Appleby.

Digest::MurmurHash compliance Digest API of Ruby.

## Usage

All interface compliance Digest::Base.

You can use same interface built in Digest::XXX classes.

```ruby
require 'digest/murmurhash'

p Digest::MurmurHash.hexdigest('murmurhash') #=> 'c709abd5'
p Digest::MurmurHash.file("./LICENSE.txt") #=> '712e9641'
```

## Class tree

Digest::MurmurHash < Digest::Base < Digest::Instance < Digest::Class < Object < Kernel < BasicObject

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
