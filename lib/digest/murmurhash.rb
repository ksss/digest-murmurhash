begin
  require "digest/#{RUBY_VERSION[/\d+.\d+/]}/murmurhash/murmurhash"
rescue LoadError
  require "digest/murmurhash/murmurhash"
end
require "digest/murmurhash/version"
