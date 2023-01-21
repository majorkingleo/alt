HOST=LINUX
HFILES=book.h db_settings.h db_struct.h fes.h notes.h sequence.h sequences.h
CFILES=book.cpp db_settings.cpp db_struct.cpp fes.cpp notes.cpp sequence.cpp sequences.cpp
OFILES=book.o db_settings.o db_struct.o fes.o notes.o sequence.o sequences.o
VERSION=0.1
CXX=g++
CXXFLAGS= -O2 -I/usr/local/include/fox-1.7  -I/home/martin/programs/alt/. -I/home/martin/programs/alt/tools -I/home/martin/programs/alt/shared -I/home/martin/programs/alt/.
LDFLAGS= -lmysqlclient -L/usr/local/lib -lFOX-1.7 -lXext -lX11 -lXcursor -lXrender -lXrandr -lXfixes -lXi -ldl -lpthread -lrt -ljpeg -lpng -ltiff -lz -lbz2 -lm -lGLU -lGL  -L. -Ltools -Lshared -L.



libshared.a: $(OFILES)
	ar r libshared.a $(OFILES)

clean:
	rm -f core* libshared.a $(OFILES)


book.o: book.cpp $(HFILES)
	$(CXX) -o book.o -c book.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

db_settings.o: db_settings.cpp $(HFILES)
	$(CXX) -o db_settings.o -c db_settings.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

db_struct.o: db_struct.cpp $(HFILES)
	$(CXX) -o db_struct.o -c db_struct.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

fes.o: fes.cpp $(HFILES)
	$(CXX) -o fes.o -c fes.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

notes.o: notes.cpp $(HFILES)
	$(CXX) -o notes.o -c notes.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

sequence.o: sequence.cpp $(HFILES)
	$(CXX) -o sequence.o -c sequence.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

sequences.o: sequences.cpp $(HFILES)
	$(CXX) -o sequences.o -c sequences.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

