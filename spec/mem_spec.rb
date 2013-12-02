require 'spec_helper'

describe Digest::MurmurHash do
  it "gc safe" do
    murmur = Digest::MurmurHash.new
    GC.start
    murmur.update("murmur")
    GC.start
    expect(murmur.update("hash").to_s).to eq("c709abd5");
  end
end

