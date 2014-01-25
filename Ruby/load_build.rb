ruby_path = File.dirname(__FILE__)
src_path = File.join(ruby_path, 'src')
solution_path = File.expand_path( File.join(ruby_path, '..') )

$LOAD_PATH << src_path

require File.join(solution_path, BUILD, 'TaskbarProgress')
require 'taskbar_progress.rb'

puts "TaskbarProgress #{TaskbarProgress::CEXT_VERSION} (#{BUILD}) loaded!"
