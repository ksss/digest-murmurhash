require "bundler/gem_tasks"

require 'rspec/core'
require 'rspec/core/rake_task'

RSpec::Core::RakeTask.new(:spec) do |t|
  t.rspec_opts = ["-c", "-f progress", "-Ilib"]
  t.pattern = "spec/**/*_spec.rb"
  t.verbose = true
end
task :spec => :compile

require 'rake/extensiontask'
spec = Bundler::GemHelper.gemspec
Rake::ExtensionTask.new('murmurhash', spec) do |ext|
  ext.ext_dir = 'ext/digest/murmurhash'
  ext.lib_dir = 'lib/digest/murmurhash'
end

desc "gem reinstall"
task :reinstall do |t|
  system "gem uninstall murmurhash"
  system "rake clean"
  system "bundle exec rake"
  system "gem install pkg/digest-murmurhash-0.2.3.gem"
end


task :default => [:spec, :build]
