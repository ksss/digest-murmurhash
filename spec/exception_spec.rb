require 'spec_helper'

describe Digest::MurmurHash do
  it "update nil" do
    murmur = Digest::MurmurHash.new
    expect{ murmur.update }.to raise_error(ArgumentError)
  end

  it "rawdigest no arguments" do
    expect{ Digest::MurmurHash.rawdigest }.to raise_error(ArgumentError)
  end
end
