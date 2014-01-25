ruby_path = File.dirname(__FILE__)
solution_path = File.expand_path( File.join(ruby_path, '..') )

require File.join(solution_path, BUILD, 'TaskbarProgress')

puts "TaskbarProgress #{TaskbarProgress::CEXT_VERSION} (#{BUILD}) loaded!"
