require 'spec_helper'

describe "Digest::MurmurHash1 and 2" do
  it "initialize" do
    expect(MurmurHash1.new).to be_a_kind_of(Digest::StringBuffer)
    expect(MurmurHash2.new).to be_a_kind_of(Digest::StringBuffer)
  end

  it "digest" do
    expect(MurmurHash1.digest("a" * 1024)).to eq("\xa1\x52\x2e\x5f".force_encoding("ASCII-8BIT"))
    expect(MurmurHash2.digest("a" * 1024)).to eq("\xd0\x0c\x31\x2f".force_encoding("ASCII-8BIT"))
  end

  it "hexdigest" do
    expect(MurmurHash1.hexdigest("a" * 1024)).to eq("a1522e5f")
    expect(MurmurHash2.hexdigest("a" * 1024)).to eq("d00c312f")
  end

  it "rawdigest" do
    expect(MurmurHash1.rawdigest("a" * 1024)).to eq(0xa1522e5f)
    expect(MurmurHash2.rawdigest("a" * 1024)).to eq(0xd00c312f)
  end

  it "update and reset and hexdigest" do
    {
      MurmurHash1 => "c709abd5",
      MurmurHash2 => "33f67c7e"
    }.each do |c, should|
      murmur = c.new
      murmur.update("m").update("u").update("r")
      murmur << "m" << "u" << "r"
      murmur << "hash"
      expect(murmur.hexdigest).to eq(should);
      expect(murmur.hexdigest).to eq(should);
      expect(murmur.hexdigest!).to eq(should);
      expect(murmur.hexdigest).to eq("00000000");
    end
  end

  it "==" do
    [MurmurHash1, MurmurHash2].each do |c|
      ["", "murmur", "murmurhash" * 1024].each do |str|
        murmur1 = c.new
        murmur2 = c.new
        expect(murmur1.update(str) == murmur2.update(str)).to be_true
      end
    end
  end

  it "dup" do
    [MurmurHash1, MurmurHash2].each do |c|
      murmur1 = c.new
      murmur2 = c.new
      10.times {
        murmur1 = murmur1.update("murmurhash" * 100).dup
      }
      murmur2.update(("murmurhash" * 100) * 10)
      expect(murmur1 == murmur2).to be_true
    end
  end

  it "length" do
    expect(MurmurHash1.new.length).to eq(4);
    expect(MurmurHash2.new.length).to eq(4);
  end

  it "to_i" do
    expect(MurmurHash1.new.update("murmurhash").to_i).to eq(0xc709abd5);
    expect(MurmurHash2.new.update("murmurhash").to_i).to eq(0x33f67c7e);
  end
end
