require 'spec_helper'

describe MurmurHash do
  let :all do
    [MurmurHash1, MurmurHash2, MurmurHash2A]
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
end
