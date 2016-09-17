PROG=ml4
OBJS=main.o \
     symbol.o \
     cek.o \
     parser.o \
     parser-cb-cek.o \
     scanner.o

PDBGOBJS=parser-dbg.o parser.o scanner.o parser-cb-str.o

#CXX=clang++
#CXX=g++
CXXFLAGS=-std=c++11 -Wall -O3

ml4 : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

scanner-test : scanner-test.cpp scanner.o
	$(CXX) $(CXXFLAGS) scanner-test.cpp scanner.o -o $@

parser-dbg : $(PDBGOBJS)
	$(CXX) $(CXXFLAGS) $(PDBGOBJS) -o $@

main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

symbol.o : symbol.cpp symbol.hpp
	$(CXX) $(CXXFLAGS) -c symbol.cpp -o $@

cek.o : cek.cpp cek.hpp
	$(CXX) $(CXXFLAGS) -c cek.cpp -o $@

parser.o : parser.cpp parser.hpp
	$(CXX) $(CXXFLAGS) -c parser.cpp -o $@

parser-cb-cek.o : parser-cb-cek.cpp parser-cb-cek.hpp
	$(CXX) $(CXXFLAGS) -c parser-cb-cek.cpp -o $@

scanner.o : scanner.cpp scanner.hpp
	$(CXX) $(CXXFLAGS) -c scanner.cpp -o $@

parser-dbg.o : parser-dbg.cpp
	$(CXX) $(CXXFLAGS) -c parser-dbg.cpp -o $@

parser-cb-str.o : parser-cb-str.cpp parser-cb-str.hpp
	$(CXX) $(CXXFLAGS) -c parser-cb-str.cpp -o $@

clean :
	$(RM) $(PROG) *.o
	$(RM) scanner-test parser-dbg
