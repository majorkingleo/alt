HOST=LINUX
HFILES=arg.h backtrace.h cppdir.h database.h db.h dbi.h debug.h format.h iterator.h leoini.h mysql_db.h odbc_db.h oracle_db.h pairs.h range.h ref.h string_utils.h theme.h thread.h xml.h
CFILES=arg.cpp backtrace.cpp cppdir.cpp database.cpp db.cpp dbi.cpp debug.cpp format.cpp leoini.cpp mysql_db.cpp odbc_db.cpp oracle_db.cpp pairs.cpp string_utils.cpp theme.cpp thread.cpp xml.cpp
OFILES=arg.o backtrace.o cppdir.o database.o db.o dbi.o debug.o format.o leoini.o mysql_db.o odbc_db.o oracle_db.o pairs.o string_utils.o theme.o thread.o xml.o
VERSION=0.1
CXX=g++
CXXFLAGS= -O2 -I/usr/local/include/fox-1.7  -I/home/martin/programs/alt/. -I/home/martin/programs/alt/tools -I/home/martin/programs/alt/shared
LDFLAGS= -lmysqlclient -L/usr/local/lib -lFOX-1.7 -lXext -lX11 -lXcursor -lXrender -lXrandr -lXfixes -lXi -ldl -lpthread -lrt -ljpeg -lpng -ltiff -lz -lbz2 -lm -lGLU -lGL  -L. -Ltools -Lshared



libtools.a: $(OFILES)
	ar r libtools.a $(OFILES)

clean:
	rm -f core* libtools.a $(OFILES)


arg.o: arg.cpp $(HFILES)
	$(CXX) -o arg.o -c arg.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

backtrace.o: backtrace.cpp $(HFILES)
	$(CXX) -o backtrace.o -c backtrace.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

cppdir.o: cppdir.cpp $(HFILES)
	$(CXX) -o cppdir.o -c cppdir.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

database.o: database.cpp $(HFILES)
	$(CXX) -o database.o -c database.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

db.o: db.cpp $(HFILES)
	$(CXX) -o db.o -c db.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

dbi.o: dbi.cpp $(HFILES)
	$(CXX) -o dbi.o -c dbi.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

debug.o: debug.cpp $(HFILES)
	$(CXX) -o debug.o -c debug.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

format.o: format.cpp $(HFILES)
	$(CXX) -o format.o -c format.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

leoini.o: leoini.cpp $(HFILES)
	$(CXX) -o leoini.o -c leoini.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

mysql_db.o: mysql_db.cpp $(HFILES)
	$(CXX) -o mysql_db.o -c mysql_db.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

odbc_db.o: odbc_db.cpp $(HFILES)
	$(CXX) -o odbc_db.o -c odbc_db.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

oracle_db.o: oracle_db.cpp $(HFILES)
	$(CXX) -o oracle_db.o -c oracle_db.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

pairs.o: pairs.cpp $(HFILES)
	$(CXX) -o pairs.o -c pairs.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

string_utils.o: string_utils.cpp $(HFILES)
	$(CXX) -o string_utils.o -c string_utils.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

theme.o: theme.cpp $(HFILES)
	$(CXX) -o theme.o -c theme.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

thread.o: thread.cpp $(HFILES)
	$(CXX) -o thread.o -c thread.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

xml.o: xml.cpp $(HFILES)
	$(CXX) -o xml.o -c xml.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

