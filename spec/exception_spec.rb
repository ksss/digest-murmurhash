require 'spec_helper'

describe MurmurHash do
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

  it "seed digest" do
    all_classes.each do |c|
      expect{ c.digest "", ""}.to raise_error(ArgumentError)
      expect{ c.digest "", "\x00" }.to raise_error(ArgumentError)
      seed = seed_str(c)
      expect{ c.digest "", seed }.to_not raise_error
    end
  end

  it "seed instance" do
    all_classes.each do |c|
      murmur = c.new
      expect{ murmur.seed = "\x00" * (c.seed_length  ) }.to_not raise_error
      expect{ murmur.seed = "\x00" * (c.seed_length+1) }.to raise_error(ArgumentError)
      seed = seed_str(c)
      expect{ murmur.seed = seed }.to_not raise_error
    end
  end
end
