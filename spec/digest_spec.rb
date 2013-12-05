require 'spec_helper'

describe MurmurHash do
  let :murmur do
    MurmurHash.new
  end

  it "initialize" do
    expect(MurmurHash.new).to be_a_kind_of(Base)
  end

  it "digest" do
    expect(MurmurHash.digest("a" * 1024)).to eq("\xa1\x52\x2e\x5f".force_encoding("ASCII-8BIT"))
  end

  it "hexdigest" do
    expect(MurmurHash.hexdigest("a" * 1024)).to eq("a1522e5f")
  end

  it "rawdigest" do
    expect(MurmurHash.rawdigest("a" * 1024)).to eq(0xa1522e5f)
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
      murmur1 = MurmurHash.new
      murmur2 = MurmurHash.new
      expect(murmur1.update(str) == murmur2.update(str)).to be_true
    end
  end

  it "dup" do
    murmur1 = MurmurHash.new
    murmur2 = MurmurHash.new
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
