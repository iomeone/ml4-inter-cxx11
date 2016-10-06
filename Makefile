PROG=ml4
OBJS=main.o \
     symbol.o \
     parser.o \
     parser-cb-cek.o \
     scanner.o \
     cek-cell.o \
     cek-env.o \
     cek-shuntingyard.o \
     cek-engine.o \
     cek-value-bool.o \
     cek-value-int.o \
     cek-closure.o \
     cek-error.o \
     cek-rt.o \
     cek-stop.o \
     cek-decl.o \
     cek-declrec.o \
     cek-let.o \
     cek-letrec.o \
     cek-fun.o \
     cek-if.o \
     cek-app.o \
     cek-prim.o \
     cek-ref.o \
     cek-blit.o \
     cek-ilit.o

CEKEVALHPP=cek-cell.hpp \
           cek-value.hpp \
           cek-ctrl.hpp \
           cek-term.hpp \
           cek-env.hpp \
           cek-kont.hpp \
           cek-shuntingyard.hpp \
           cek-engine.hpp \
           cek-decl.hpp \
           cek-declrec.hpp \
           cek-let.hpp \
           cek-letrec.hpp \
           cek-fun.hpp \
           cek-if.hpp \
           cek-app.hpp \
           cek-prim.hpp \
           cek-ref.hpp \
           cek-blit.hpp \
           cek-ilit.hpp

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

main.o : main.cpp parser.hpp parser-cb-cek.hpp cek-eval.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

symbol.o : symbol.cpp symbol.hpp
	$(CXX) $(CXXFLAGS) -c symbol.cpp -o $@

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

cek-eval.hpp : $(CEKEVALHPP)

cek-app.o : cek-app.cpp cek-eval.hpp cek-app.hpp cek-app1-kont.hpp cek-app2-kont.hpp cek-error.hpp
	$(CXX) $(CXXFLAGS) -c cek-app.cpp -o $@

cek-blit.o : cek-blit.cpp cek-eval.hpp cek-blit.hpp cek-value-bool.hpp cek-rt-ctrl.hpp
	$(CXX) $(CXXFLAGS) -c cek-blit.cpp -o $@

cek-cell.o : cek-cell.cpp cek-eval.hpp
	$(CXX) $(CXXFLAGS) -c cek-cell.cpp -o $@

cek-closure.o : cek-closure.cpp cek-eval.hpp cek-closure.hpp
	$(CXX) $(CXXFLAGS) -c cek-closure.cpp -o $@

cek-decl.o : cek-decl.cpp cek-eval.hpp cek-decl.hpp cek-decl-kont.hpp cek-rt-kont.hpp
	$(CXX) $(CXXFLAGS) -c cek-decl.cpp -o $@

cek-declrec.o : cek-declrec.cpp cek-eval.hpp cek-declrec.hpp cek-declrec-kont.hpp cek-rt-kont.hpp
	$(CXX) $(CXXFLAGS) -c cek-declrec.cpp -o $@

cek-engine.o : cek-engine.cpp cek-eval.hpp cek-blit.hpp cek-stop.hpp cek-rt-kont.hpp
	$(CXX) $(CXXFLAGS) -c cek-engine.cpp -o $@

cek-env.o : cek-env.cpp cek-eval.hpp
	$(CXX) $(CXXFLAGS) -c cek-env.cpp -o $@

cek-error.o : cek-error.cpp cek-eval.hpp cek-error.hpp cek-rt-ctrl.hpp
	$(CXX) $(CXXFLAGS) -c cek-error.cpp -o $@

cek-fun.o : cek-fun.cpp cek-eval.hpp cek-fun.hpp cek-closure.hpp cek-rt-ctrl.hpp
	$(CXX) $(CXXFLAGS) -c cek-fun.cpp -o $@

cek-if.o : cek-if.cpp cek-eval.hpp cek-if.hpp cek-if-kont.hpp
	$(CXX) $(CXXFLAGS) -c cek-if.cpp -o $@

cek-ilit.o : cek-ilit.cpp cek-eval.hpp cek-ilit.hpp cek-value-int.hpp cek-rt-ctrl.hpp
	$(CXX) $(CXXFLAGS) -c cek-ilit.cpp -o $@

cek-let.o : cek-let.cpp cek-eval.hpp cek-let.hpp cek-let-kont.hpp
	$(CXX) $(CXXFLAGS) -c cek-let.cpp -o $@

cek-letrec.o : cek-letrec.cpp cek-eval.hpp cek-letrec.hpp cek-letrec-kont.hpp
	$(CXX) $(CXXFLAGS) -c cek-letrec.cpp -o $@

cek-prim.o : cek-prim.cpp cek-eval.hpp cek-prim.hpp cek-prim1-kont.hpp cek-prim2-kont.hpp cek-rt-ctrl.hpp cek-error.hpp
	$(CXX) $(CXXFLAGS) -c cek-prim.cpp -o $@

cek-ref.o : cek-ref.cpp cek-eval.hpp cek-ref.hpp cek-rt-ctrl.hpp cek-error.hpp
	$(CXX) $(CXXFLAGS) -c cek-ref.cpp -o $@

cek-rt.o : cek-rt.cpp cek-eval.hpp cek-rt-ctrl.hpp cek-rt-kont.hpp
	$(CXX) $(CXXFLAGS) -c cek-rt.cpp -o $@

cek-shuntingyard.o : cek-shuntingyard.cpp cek-eval.hpp
	$(CXX) $(CXXFLAGS) -c cek-shuntingyard.cpp -o $@

cek-stop.o : cek-stop.cpp cek-eval.hpp cek-stop.hpp
	$(CXX) $(CXXFLAGS) -c cek-stop.cpp -o $@

cek-value-bool.o : cek-value-bool.cpp cek-eval.hpp cek-value-bool.hpp
	$(CXX) $(CXXFLAGS) -c cek-value-bool.cpp -o $@

cek-value-int.o : cek-value-int.cpp cek-eval.hpp cek-value-int.hpp
	$(CXX) $(CXXFLAGS) -c cek-value-int.cpp -o $@

clean :
	$(RM) $(PROG) *.o
	$(RM) scanner-test parser-dbg
