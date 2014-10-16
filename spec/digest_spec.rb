require 'spec_helper'

describe "Digest::MurmurHash" do
  it "seed" do
    all_classes.each do |c|
      m = c.new
      expect(c::DEFAULT_SEED).to eq(m.seed)
    end
  end

  it "seed=" do
    all_classes.each do |c|
      m = c.new
      m.update "murmurhash"
      before_digest = m.hexdigest
      m.seed = seed_str(c)
      expect(c::DEFAULT_SEED).not_to eq(m.seed)
      expect(before_digest).not_to eq(m.hexdigest)
    end
  end

  it "initialize" do
    all_classes.each do |c|
      expect(c.new).to be_a_kind_of(Digest::Class)
    end
  end

  context "static hexdigest" do
    it { expect(Digest::MurmurHash1.hexdigest("abc")).to eq("dc5f5755") }
    it { expect(Digest::MurmurHash2.hexdigest("abc")).to eq("9b7c5713") }
    it { expect(Digest::MurmurHash2A.hexdigest("abc")).to eq("679f5811") }
    it { expect(Digest::MurmurHash64A.hexdigest("abc")).to eq("fb5ea99834c3c99c") }
    it { expect(Digest::MurmurHash64B.hexdigest("abc")).to eq("9d595cce51420da6") }
    it { expect(Digest::MurmurHashAligned2.hexdigest("abc")).to eq("9b7c5713") }
    it { expect(Digest::MurmurHashNeutral2.hexdigest("abc")).to eq("9b7c5713") }
    it { expect(Digest::MurmurHash3_x86_32.hexdigest("abc")).to eq("fa93ddb3") }
    it { expect(Digest::MurmurHash3_x86_128.hexdigest("abc")).to eq("a506b0a2a506b0a2a506b0a2d1c6cd75") }
    it { expect(Digest::MurmurHash3_x64_128.hexdigest("abc")).to eq("4174a23b522dca263f3f96b46778ad3f") }
  end

  context "digest and hexdigest" do
    all_classes.each do |c|
      [:digest, :hexdigest].each do |method|
        str = "a" * 1024
        d = c.send(method, str)
        d2 = c.send(method, str, seed_str(c))
        it ("#{c}##{method}") do
          expect(d).to be_a_kind_of(String)
          expect(d2).to be_a_kind_of(String)
          expect(d.length).to be > 0
          expect(d2.length).to be > 0
          expect(d != d2).to be_truthy
        end
      end
    end
  end

  describe "rawdigest" do
    all_classes.each do |c|
      str = "a" * 1024
      seed = seed_str(c)
      d = c.rawdigest str
      d2 = c.rawdigest str, seed
      it(c) do
        expect(d != d2).to be_truthy
      end
    end
  end

  describe "update and reset and hexdigest" do
    all_classes.each do |c|
      murmur = c.new
      murmur.update("m").update("u").update("r")
      murmur << "m" << "u" << "r"
      murmur << "hash"
      hex = murmur.hexdigest
      it(c) do
        expect(murmur.hexdigest! == hex).to be true
        expect(murmur.hexdigest).to eq("0" * (murmur.digest_length*2))
      end
    end
  end

  it "==" do
    all_classes.each do |c|
      ["", "murmur", "murmurhash" * 1024].each do |str|
        murmur1 = c.new
        murmur2 = c.new
        expect(murmur1.update(str) == murmur2.update(str)).to be_truthy
      end
    end
  end

  it "dup" do
    all_classes.each do |c|
      murmur1 = c.new
      murmur2 = c.new
      10.times {
        murmur1 = murmur1.update("murmurhash" * 100).dup
      }
      murmur2.update(("murmurhash" * 100) * 10)
      expect(murmur1).to eq(murmur2)
    end
  end

  it "reset" do
    all_classes.each do |c|
      m1 = c.new
      m2 = c.new
      m1.update("murmurhash")
      m1.seed = seed_str(c)
      expect(m1.reset).to eq(m2)
    end
  end
end
