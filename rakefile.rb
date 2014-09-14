# -----------------------------------------------------------------------------
require 'rake/clean'
require 'rake/tasklib'

# -----------------------------------------------------------------------------
require './rakelib/lib/ctasklib'

# -----------------------------------------------------------------------------
case ENV["variant"]
when "release"
    ENV["CFLAGS"]  = %q(-O2 -Wall -MMD)
    ENV["LDFLAGS"] = %q(-pthread)
else
    ENV["CFLAGS"]  = %q(-g -Wall -MMD)
    ENV["LDFLAGS"] = %q(-g -pthread)
end

ENV["CPPFLAGS"] = %q(-std=c++11)

# -----------------------------------------------------------------------------
json = Rake::StaticLibraryTask.new("json.yml")

# -----------------------------------------------------------------------------
spec = Rake::ExecutableSpecification.new do |s|
    s.name = 'test'
    s.includes.add %w(
        src
        include
        test/catch
    )
    s.libincludes.add %w(
        build
    )
    s.sources.add %w(
        test/**/*.cpp

    )
    s.libraries += [ json ]
end

# -----------------------------------------------------------------------------
Rake::ExecutableTask.new(:test, spec)

# -----------------------------------------------------------------------------
CLEAN.include('build')
# -----------------------------------------------------------------------------
task :default => [ :test ]
task :all => [ :default ]
