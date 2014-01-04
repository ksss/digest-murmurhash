require 'spec_helper'

describe MurmurHash do
  let :all do
    [MurmurHash1, MurmurHash2, MurmurHash2A, MurmurHash64A, MurmurHash64B]
  end

  let :seed32 do
    (0..4).to_a.pack("C4")
  end

  let :seed64 do
    (0..8).to_a.pack("C8")
  end

  it "update nil" do
    all.each do |c|
      murmur = c.new
      expect{ murmur.update }.to raise_error(ArgumentError)
    end
  end

  it "rawdigest no arguments" do
    all.each do |c|
      expect{ c.rawdigest }.to raise_error(ArgumentError)
    end
  end

  it "seed digest" do
    all.each do |c|
      expect{ c.digest "", ""}.to raise_error(ArgumentError)
      expect{ c.digest "", "\x00" }.to raise_error(ArgumentError)
      seed = (c::DEFAULT_SEED.bytesize == 4) ? seed32 : seed64
      expect{ c.digest "", seed }.to_not raise_error
    end
  end

  it "seed instance" do
    all.each do |c|
      murmur = c.new
      expect{ murmur.seed = "" }.to raise_error(ArgumentError)
      expect{ murmur.seed = "\x00" }.to raise_error(ArgumentError)
      seed = (c::DEFAULT_SEED.bytesize == 4) ? seed32 : seed64
      expect{ murmur.seed = seed }.to_not raise_error
    end
  end
end
