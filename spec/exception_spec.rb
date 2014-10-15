require 'spec_helper'

describe "Digest::MurmurHash" do
  it "update nil" do
    all_classes.each do |c|
      murmur = c.new
      expect{ murmur.update }.to raise_error(ArgumentError)
    end
  end

  it "rawdigest no arguments" do
    all_classes.each do |c|
      expect{ c.rawdigest }.to raise_error(ArgumentError)
    end
  end

  describe "seed digest" do
    all_classes.each do |c|
      it (c) do
        expect{ c.digest "", ""}.to raise_error(ArgumentError)
        expect{ c.digest "", "\x00" * (c::DEFAULT_SEED.length  )}.to_not raise_error
        expect{ c.digest "", "\x00" * (c::DEFAULT_SEED.length+1)}.to raise_error(ArgumentError)
        expect{ c.digest "", seed_str(c) }.to_not raise_error
      end
    end
  end

  describe "seed instance" do
    all_classes.each do |c|
      it (c) do
        murmur = c.new
        expect{ murmur.seed = "" }.to raise_error(ArgumentError)
        expect{ murmur.seed = "\x00" * (c::DEFAULT_SEED.length  ) }.to_not raise_error
        expect{ murmur.seed = "\x00" * (c::DEFAULT_SEED.length+1) }.to raise_error(ArgumentError)
        seed = seed_str(c)
        expect{ murmur.seed = seed }.to_not raise_error
      end
    end
  end
end
