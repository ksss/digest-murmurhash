require 'spec_helper'

describe Digest::MurmurHash do
  it "seed" do
    all.each do |c|
      m = c.new
      expect(c::DEFAULT_SEED == m.seed).to be true
    end
  end

  it "seed=" do
    all.each do |c|
      m = c.new
      m.update "murmurhash"
      before_digest = m.hexdigest
      m.seed = (c::DEFAULT_SEED.length == 4) ? seed32 : seed64
      expect(c::DEFAULT_SEED != m.seed).to be true
      expect(before_digest != m.hexdigest).to be true
    end
  end

  it "initialize" do
    all.each do |c|
      expect(c.new).to be_a_kind_of(Digest::StringBuffer)
    end
  end

  it "digest and hexdigest" do
    all.each do |c|
      [:digest, :hexdigest].each do |method|
        str = "a" * 1024
        d = c.send(method, str)
        d2 = c.send(method, str, (c::DEFAULT_SEED.length == 4) ? seed32 : seed64)
        expect(d).to be_a_kind_of(String)
        expect(d2).to be_a_kind_of(String)
        expect(d.length).to be > 0
        expect(d2.length).to be > 0
        expect(d != d2).to be true
      end
    end
  end

  it "rawdigest" do
    all.each do |c|
      str = "a" * 1024
      d = c.rawdigest str
      d2 = c.rawdigest str, (c::DEFAULT_SEED.length == 4) ? seed32 : seed64
      expect(d).to be_a_kind_of(Integer)
      expect(d2).to be_a_kind_of(Integer)
      expect(d).to be > 0
      expect(d2).to be > 0
      expect(d != d2).to be true
    end
  end

  it "update and reset and hexdigest(32bit)" do
    all.each do |c|
      murmur = c.new
      murmur.update("m").update("u").update("r")
      murmur << "m" << "u" << "r"
      murmur << "hash"
      hex = murmur.hexdigest
      expect(murmur.hexdigest! == hex).to be true
      reset_str = "0" * ((c::DEFAULT_SEED.length == 4) ? 8 : 16)
      expect(murmur.hexdigest).to eq(reset_str)
    end
  end

  it "==" do
    all.each do |c|
      ["", "murmur", "murmurhash" * 1024].each do |str|
        murmur1 = c.new
        murmur2 = c.new
        expect(murmur1.update(str) == murmur2.update(str)).to be_true
      end
    end
  end

  it "dup" do
    all.each do |c|
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
    all.each do |c|
      expect(c.new.length == c::DEFAULT_SEED.length).to be true
    end
  end

  it "to_i" do
    all.each do |c|
      expect(c.new.update("murmurhash").to_i).to be_a_kind_of(Integer)
    end
  end
end
