require "digest/simple"

module Digest
  class MurmurHash < Simple
    def initialize
      super
      reset
    end

    def seed
      @seed
    end

    def seed=(s)
      fail ArgumentError, "seed string should #{digest_length * 16} bit chars" if s.bytesize != digest_length
      @seed = s
    end

    def reset
      super
      @seed = self.class::DEFAULT_SEED
      self
    end
  end

  {
    '1' => 32,
    '2' => 32,
    '2A' => 32,
    '64A' => 64,
    '64B' => 64,
    'Aligned2' => 32,
    'Neutral2' => 32,
  }.each do |name, size|
    class_eval %Q{
      class MurmurHash#{name} < MurmurHash
        DEFAULT_SEED = "#{"\x00" * (size/8)}".encode('ASCII-8BIT')
        def digest_length
          #{size/8}
        end

        def to_i
          finish.unpack("#{size == 32 ? "I" : size == 64 ? "L" : "Q"}")[0]
        end
      end
    }
  end
end

require "digest/murmurhash/murmurhash"
