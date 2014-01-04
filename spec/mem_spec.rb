require 'spec_helper'

describe MurmurHash do
  let :all do
    [MurmurHash1, MurmurHash2, MurmurHash2A, MurmurHash64A, MurmurHash64B]
  end

  it "gc safe" do
    all.each do |c|
      murmur = c.new
      init = murmur.to_s
      GC.start
      murmur.update("murmur")
      GC.start
      expect(murmur.update("hash").to_s != init).to be true
    end
  end
end

