#!/usr/bin/env ruby

# LALR(1) grammar table of ML<sup>4</sup> language

# Toy LALR(1) Parser https://github.com/tociyuki/rzubr-ruby
require 'rzubr'

cols = [
  Rzubr::ENDMARK,
  ";;", "in", "then", "else", ")", "+", "<", "*", "=", "->", "rec",
  "let", "if", "fun", "id", "int", "bool", "("]
colg = [
  :aexpr, :app, :mexpr, :funexpr, :pexpr, :expr, :prog]

def grammar_table
  rule = Rzubr::Rule

  prog = rule.name(:prog) \
    > rule[:expr, ';;'] \
    | rule['let', 'id', '=', :expr, ';;'] \
    | rule['let', 'rec', 'id', '=', :funexpr, ';;']

  expr = rule.name(:expr) \
    > rule['let', 'id', '=', :expr, 'in', :expr] \
    | rule['let', 'rec', 'id', '=', :funexpr, 'in', :expr] \
    | rule['if', :expr, 'then', :expr, 'else', :expr] \
    | rule[:funexpr] \
    | rule[:pexpr] \
    | rule[:pexpr, '<', :pexpr]

  funexpr = rule.name(:funexpr) \
    > rule['fun', 'id', '->', :expr]

  pexpr = rule.name(:pexpr) \
    > rule[:pexpr, '+', :mexpr] \
    | rule[:mexpr]

  mexpr = rule.name(:mexpr) \
    > rule[:mexpr, '*', :app] \
    | rule[:app]

  app = rule.name(:app) \
    > rule[:app, :aexpr] \
    | rule[:aexpr]

  aexpr = rule.name(:aexpr) \
    > rule['id'] \
    | rule['int'] \
    | rule['bool'] \
    | rule['(', :expr, ')']

  g = (prog + expr + funexpr + pexpr + mexpr + app + aexpr).start(:prog)
  Rzubr::LALR1.new.rule(g)
end

def build_reduce_table(cols, colg, lr)
  prod = lr.production
  (1 ... prod.size).map{|i|
    ((cols.size + colg.index(prod[i].lhs)) << 8) | prod[i].rhs.size
  }
end

def build_lr_table(cols, colg, lr)
  prod = lr.production
  table = []
  lr.state.term.each_index do |state_p|
    row = []
    cols.each do |s|
      if not lr.action[state_p].key?(s)
        row << nil
        next
      end
      x = lr.action[state_p][s]
      case x
      when Integer
        row << x + 1
      when :accept
        row << 0xff
      when Rzubr::Production
        row << (0xfe - prod.index(x) + 1)
      end
    end
    colg.each do |s|
      if not lr.goto[state_p].key?(s)
        row << nil
        next
      end
      x = lr.goto[state_p][s]
      row << x + 1
    end
    table << row
  end
  table
end

def compact_table(table)
  base = [0]
  check = []
  state_p = 1
  table.each do |row|
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

def list_table(prod, table)
  (1 ... prod.size).each do |i|
    e = prod[i]
    t = [
      e.lhs.to_s,
      e.rhs.map {|s| Symbol === s ? s.to_s : s.inspect }.join(' ')].join(': ')
    puts '// %2d %02x %s' % [i - 1, 0xfe - (i - 1), t]
  end
  puts
  table.each_with_index do |row,state_p|
    puts '// %02x %s' % [state_p + 1,
      row.map{|x| x.nil? ? '. ' : '%02x' % x }.join(' ')]
  end
end

def list_compact_table(reduce, base, check)
  puts 'static const unsigned short PRODUCTION[] = {'
  reduce.each_slice(8) do |a|
    puts '    ' + a.map{|x| '%#06x,' % [x] }.join(' ')
  end
  puts '};'
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
    puts '    ' + a.map{|x| (0 == x ? '%6d,' : '%#06x,') % [x] }.join(' ')
  end
  puts '};'
end

lr = grammar_table
reduce = build_reduce_table(cols, colg, lr)
table = build_lr_table(cols, colg, lr)
list_table(lr.production, table)
base, check = compact_table(table)
puts
list_compact_table(reduce, base, check)
