DESTDIR=
PREFIX=/usr
BINDIR=/bin
CFLAGS?= -fPIC #-O1

everything: bin/radamsa

bin/radamsa:
	mkdir -p bin
	c++ $(CFLAGS) -g -o bin/radamsa.so -shared radamsa.c radamsa.cc
	-cp radamsa.py bin/radamsa.py
bin/radamsa_standalone:
	mkdir -p bin
	c++ $(CFLAGS) -g -o bin/radamsa_standalone radamsa.c radamsa.cc

clean:
	-rm bin/radamsa.so 
	-rm bin/radamsa_standalone
	-rm bin/radamsa.py
