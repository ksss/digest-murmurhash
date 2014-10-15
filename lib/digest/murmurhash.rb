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
      fail ArgumentError, "seed string should be #{self.class.seed_length} length" if self.class.seed_length < s.length
      @seed = s
    end

    def reset
      super
      @seed = self.class::DEFAULT_SEED
      self
    end
  end

  ds = Struct.new(:digest_length, :seed_length)
  s1 = ds.new(4, 4)
  s2 = ds.new(8, 8)
  s3 = ds.new(16, 4)
  {
    '1' => s1,
    '2' => s1,
    '2A' => s1,
    '64A' => s2,
    '64B' => s2,
    'Aligned2' => s1,
    'Neutral2' => s1,
    '3_x86_32' => s1,
    '3_x86_128' => s3,
    '3_x64_128' => s3,
  }.each do |name, s|
    class_eval %Q{
      class MurmurHash#{name} < MurmurHash
        DEFAULT_SEED = "#{"\x00" * s.seed_length}".encode('ASCII-8BIT')

        class << self
          def seed_length
            #{s.seed_length}
          end
        end

        def digest_length
          #{s.digest_length}
        end

        def to_i
          self.class.rawdigest(@buffer, seed)
        end
      end
    }
  end
end

require "digest/murmurhash/murmurhash"
