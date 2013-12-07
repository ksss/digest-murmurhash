begin
  require "digest/murmurhash/#{RUBY_VERSION[/\d+.\d+/]}/murmurhash"
rescue LoadError
  require "digest/murmurhash/murmurhash"
end
require "digest/murmurhash/version"
