# coding: utf-8

Gem::Specification.new do |spec|
  spec.name          = "digest-murmurhash"
  spec.version       = "1.1.1"
  spec.authors       = ["ksss"]
  spec.email         = ["co000ri@gmail.com"]
  spec.description   = %q{digest-murmurhash is class collections of use algorithm MurmurHash desiged by Austin Appleby.}
  spec.summary       = %q{digest-murmurhash is class collections of use algorithm MurmurHash desiged by Austin Appleby.}
  spec.homepage      = "https://github.com/ksss/digest-murmurhash"
  spec.license       = "MIT"

  spec.files         = `git ls-files`.split($/)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]
  spec.extensions    = ["ext/digest/murmurhash/extconf.rb"]

  spec.add_development_dependency "bundler"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rspec"
  spec.add_development_dependency "rake-compiler"
end
