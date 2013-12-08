require 'spec_helper'

describe MurmurHash do
  it "gc safe" do
    {
      MurmurHash1 => "c709abd5",
      MurmurHash2 => "33f67c7e",
      MurmurHash2A => "df25554b",
    }.each do |c, should|
      murmur = c.new
      GC.start
      murmur.update("murmur")
      GC.start
      expect(murmur.update("hash").to_s).to eq(should);
    end
  end
end

