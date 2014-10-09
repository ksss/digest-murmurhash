require "digest/simple"

module Digest
  class MurmurHash < Simple
    def initialize
      @seed = self.class::DEFAULT_SEED
      super
    end

    def seed
      @seed
    end

    def seed=(s)
      fail ArgumentError, "seed string should #{digest_length * 16} bit chars" if s.bytesize != digest_length
      @seed = s
    end

    module Size32
      DEFAULT_SEED = "\x00\x00\x00\x00".encode('ASCII-8BIT')

      def digest_length
        4
      end

      def to_i
        finish.unpack("I")[0]
      end
    end

    module Size64
      DEFAULT_SEED = "\x00\x00\x00\x00\x00\x00\x00\x00".encode('ASCII-8BIT')

      def digest_length
        8
      end

      def to_i
        finish.unpack("L")[0]
      end
    end
  end

  class MurmurHash1 < MurmurHash
    include Size32
  end

  class MurmurHash2 < MurmurHash
    include Size32
  end

  class MurmurHash2A < MurmurHash
    include Size32
  end

  class MurmurHash64A < MurmurHash
    include Size64
  end

  class MurmurHash64B < MurmurHash
    include Size64
  end

  class MurmurHashAligned2 < MurmurHash
    include Size32
  end

  class MurmurHashNeutral2 < MurmurHash
    include Size32
  end
end

require "digest/murmurhash/murmurhash"
