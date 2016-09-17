#!/usr/bin/env ruby

# DFA rule for the lexical scanner of ML<sup>4</sup> language

rule = [
#    1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14
  [nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil], #
  [nil, 16, 15, 18,  5,  8,  6,nil,nil,  7,  9,  0,  1, 10], # #
  [nil,nil,  3,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil], # _
  [nil,nil,nil,nil,nil,nil,nil, 14,nil,nil,nil,nil,nil,nil], # >
  [  2,  2,  3,nil,nil,nil,nil,  2,nil,nil,nil,nil,nil,nil], # 0-9
  [  3,nil,  3,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil], # a-z
  [  4,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil], # (
  [  5,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil], # )
  [  6,nil,nil, 12,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil], # *
  [  7,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil], # +
  [  8,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil], # -
  [  9,nil,nil,nil,nil,nil,nil,nil, 13,nil,nil,nil,nil,nil], # ;
  [ 10,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil], # <
  [ 11,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil], # =
  [  1,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil], # [\t\n ]
].transpose

def compact_table(rule)
  base = [0]
  check = []
  state_p = 1
  rule.each do |row|
    b = row.index{|x| not x.nil? }
    d = row.rindex{|x| not x.nil? }
    base[state_p] = -b
    until (b .. d).all?{|i| row[i].nil? or check[base[state_p] + i].nil? }
      base[state_p] += 1
    end
    (b .. d).each do |i|
      row[i].nil? or check[base[state_p] + i] = (row[i] << 8) + state_p
    end
    state_p += 1
  end
  check.each_index {|i| check[i] ||= 0 }
  [base, check]
end

def list_compact_table(base, check)
  puts 'static const short BASE[] = {'
  print '    %d' % [base[0]]
  col = 4 + base[0].to_s.size
  (1 ... base.size).each do |i|
    x = base[i]
    if col + x.to_s.size + 2 < 76
      print ', %d' % [x]
      col += x.to_s.size + 2
    else
      print ",\n    %d" % [x]
      col = 4
    end
  end
  puts
  puts '};'
  puts 'static const unsigned short RULE[] = {'
  check.each_slice(8) do |a|
    puts '    ' + a.map{|x| '%#06x,' % [x] }.join(' ')
  end
  puts '};'
end

base, check = compact_table(rule)
list_compact_table(base, check)
