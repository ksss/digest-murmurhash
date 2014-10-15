require 'spec_helper'

describe "Digest::MurmurHash" do
  it "gc safe" do
    all_classes.each do |c|
      murmur = c.new
      init = murmur.to_s
      GC.start
      murmur.update("murmur")
      GC.start
      expect(murmur.update("hash").to_s != init).to be true
    end
  end
end

