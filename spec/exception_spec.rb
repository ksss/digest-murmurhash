require 'spec_helper'

describe MurmurHash do
  let :all do
    [MurmurHash1, MurmurHash2, MurmurHash2A, MurmurHash64A]
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
    expect{ MurmurHash2A.digest "", ""}.to raise_error(ArgumentError)
    expect{ MurmurHash2A.digest "", "\x00" }.to raise_error(ArgumentError)
    expect{ MurmurHash2A.digest "", seed32 }.to_not raise_error

    expect{ MurmurHash64A.digest "", ""}.to raise_error(ArgumentError)
    expect{ MurmurHash64A.digest "", "\x00" }.to raise_error(ArgumentError)
    expect{ MurmurHash64A.digest "", seed64 }.to_not raise_error
  end

  it "seed instance" do
    murmur64a = MurmurHash64A.new
    expect{ murmur64a.seed = "" }.to raise_error(ArgumentError)
    expect{ murmur64a.seed = "\x00" }.to raise_error(ArgumentError)
    expect{ murmur64a.seed = seed64 }.to_not raise_error
  end
end
