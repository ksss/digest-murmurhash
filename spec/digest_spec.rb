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
    murmur.update("m").update("u").update("r")
    murmur << "m" << "u" << "r"
    murmur << "hash"
    expect(murmur.hexdigest).to eq("c709abd5");
    expect(murmur.hexdigest).to eq("c709abd5");
    expect(murmur.hexdigest!).to eq("c709abd5");
    expect(murmur.hexdigest).to eq("00000000");
  end

  it "==" do
    ["", "murmur", "murmurhash" * 1024].each do |str|
      murmur1 = Digest::MurmurHash.new
      murmur2 = Digest::MurmurHash.new
      expect(murmur1.update(str) == murmur2.update(str)).to be_true
    end
  end

  it "dup" do
    murmur1 = Digest::MurmurHash.new
    murmur2 = Digest::MurmurHash.new
    10.times {
      murmur1 = murmur1.update("murmurhash" * 100).dup
    }
    murmur2.update(("murmurhash" * 100) * 10)
    expect(murmur1 == murmur2).to be_true
  end

  it "length" do
    expect(murmur.length).to eq(4);
  end

  it "block_length" do
    # MurmurHash don't use block.
    # Therefore `block_length` return chunk size for calculate MurmurHash (equal 4) 
    expect(murmur.block_length).to eq(4);
  end

  it "to_i" do
    expect(murmur.update("murmurhash").to_i).to eq(0xc709abd5);
  end
end
