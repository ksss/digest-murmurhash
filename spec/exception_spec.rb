require 'spec_helper'

describe MurmurHash do
  it "update nil" do
    murmur = MurmurHash.new
    expect{ murmur.update }.to raise_error(ArgumentError)
  end

  it "rawdigest no arguments" do
    expect{ MurmurHash.rawdigest }.to raise_error(ArgumentError)
  end
end
