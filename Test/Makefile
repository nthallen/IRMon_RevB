LDFLAGS=-L/usr/local/lib
LIBS=-ldasio -lsubbuspp -lnl
test_ir : test_ir.o test_iroui.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o test_ir test_ir.o test_iroui.o $(LIBS)
test_iroui.o : test_iroui.cc
test_iroui.cc : test_ir.oui
	oui -o test_iroui.cc test_ir.oui