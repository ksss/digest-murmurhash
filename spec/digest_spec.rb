require 'spec_helper'

describe Digest::MurmurHash do
  let :murmur do
    Digest::MurmurHash.new
  end

  it "initialize" do
    expect(Digest::MurmurHash.new).to be_a_kind_of(Digest::Base)
  end

  it "hexdigest" do
    expect(Digest::MurmurHash.hexdigest("a" * 1024)).to eq("a1522e5f")
  end

  it "update and reset and hexdigest" do
    murmur.update("m")
    murmur.update("u")
    murmur.update("r")
    murmur.update("m")
    murmur.update("u")
    murmur.update("r")
    murmur.update("hash")
    expect(murmur.hexdigest).to eq("c709abd5");
    expect(murmur.hexdigest).to eq("c709abd5");
    expect(murmur.hexdigest!).to eq("c709abd5");
    expect(murmur.hexdigest).to eq("00000000");
  end

  it "dup" do
    murmur1 = Digest::MurmurHash.new
    murmur2 = Digest::MurmurHash.new
    murmur1.update("murmur")
    murmur2 = murmur1.dup
    murmur2.update("hash")
    expect(murmur2.hexdigest).to eq("c709abd5")
  end

  it "==" do
    ["", "murmur", "murmurhash" * 1024].each do |str|
      murmur1 = Digest::MurmurHash.new
      murmur2 = Digest::MurmurHash.new
      expect(murmur1.update(str) == murmur2.update(str)).to be_true
    end
  end

  it "length" do
    expect(murmur.length).to eq(4);
  end

  it "block_length" do
    # MurmurHash don't use block.
    # Therefore `block_length` return chunk size for calculate MurmurHash (equal 4) 
    expect(murmur.block_length).to eq(4);
  end
end
