# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
# require 'digest/murmurhash/version'

Gem::Specification.new do |spec|
  spec.name          = "digest-murmurhash"
  # FIXME replace Digest::MurmurHash::VERSION but raise NameError
  spec.version       = "0.0.1"
  spec.author        = "ksss"
  spec.email         = "co000ri@gmail.com"
  spec.description   = %q{The MurmurHash.}
  spec.summary       = %q{The MurmurHash create by Austin Appleby.}
  spec.homepage      = ""
  spec.license       = "MIT"

  spec.files         = `git ls-files`.split($/)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]
  spec.extensions    = ["ext/digest/murmurhash/extconf.rb"]

  spec.add_development_dependency "bundler", "~> 1.3"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rspec", ['~> 2.11']
  spec.add_development_dependency "rake-compiler", ["~> 0.8.3"]
end
