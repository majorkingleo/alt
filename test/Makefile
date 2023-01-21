HOST=LINUX
HFILES=
CFILES=test.cpp
OFILES=test.o
VERSION=0.1
CXX=g++
CXXFLAGS= -O2 -I/usr/local/include/fox-1.7  -I/home/martin/programs/alt/. -I/home/martin/programs/alt/tools -I/home/martin/programs/alt/shared
LDFLAGS= -lmysqlclient -L/usr/local/lib -lFOX-1.7 -lXext -lX11 -lXcursor -lXrender -lXrandr -lXfixes -lXi -ldl -lpthread -lrt -ljpeg -lpng -ltiff -lz -lbz2 -lm -lGLU -lGL  -L. -Ltools -Lshared


all: test

test: $(OFILES) ../tools/libtools.a
	$(CXX) -o test $(OFILES) $(CXXFLAGS) $(LDFLAGS) ../tools/libtools.a

clean:
	rm -f core* test $(OFILES)


test.o: test.cpp $(HFILES)
	$(CXX) -o test.o -c test.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

