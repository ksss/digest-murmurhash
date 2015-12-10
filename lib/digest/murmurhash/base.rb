module Digest
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
      class MurmurHash#{name} < Digest::Class
        DEFAULT_SEED = "#{"\x00" * s.seed_length}".b

        def initialize
          @buffer = ""
          @seed = DEFAULT_SEED
        end

        def update(str)
          @buffer << str
          self
        end
        alias << update

        def reset
          @buffer.clear
          @seed = DEFAULT_SEED
          self
        end

        def seed
          @seed
        end

        def seed=(s)
          raise ArgumentError, "seed string should be #{s.seed_length} length" if #{s.seed_length} != s.length
          @seed = s
        end

        def digest_length
          #{s.digest_length}
        end

        def block_length
          0
        end
      end
    }
  end
end
