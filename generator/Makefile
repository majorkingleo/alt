HOST=LINUX
HFILES=elements2.h generator.h
CFILES=elements2.cpp generator.cpp gen_gui_code.cpp gen_sql_code.cpp gen_structs.cpp
OFILES=elements2.o generator.o gen_gui_code.o gen_sql_code.o gen_structs.o
VERSION=0.1
CXX=g++
CXXFLAGS= -O2 -I/usr/local/include/fox-1.7  -I/home/martin/programs/alt/. -I/home/martin/programs/alt/tools -I/home/martin/programs/alt/shared
LDFLAGS= -lmysqlclient -L/usr/local/lib -lFOX-1.7 -lXext -lX11 -lXcursor -lXrender -lXrandr -lXfixes -lXi -ldl -lpthread -lrt -ljpeg -lpng -ltiff -lz -lbz2 -lm -lGLU -lGL  -L. -Ltools -Lshared


all: generator

generator: $(OFILES) ../tools/libtools.a
	$(CXX) -o generator $(OFILES) $(CXXFLAGS) $(LDFLAGS) ../tools/libtools.a

clean:
	rm -f core* generator $(OFILES)

elements2.o: elements2.cpp $(HFILES)
	$(CXX) -o elements2.o -c elements2.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

generator.o: generator.cpp $(HFILES)
	$(CXX) -o generator.o -c generator.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

gen_gui_code.o: gen_gui_code.cpp $(HFILES)
	$(CXX) -o gen_gui_code.o -c gen_gui_code.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

gen_sql_code.o: gen_sql_code.cpp $(HFILES)
	$(CXX) -o gen_sql_code.o -c gen_sql_code.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

gen_structs.o: gen_structs.cpp $(HFILES)
	$(CXX) -o gen_structs.o -c gen_structs.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

