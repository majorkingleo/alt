HOST=LINUX
HFILES=dbentry.h db_util.h i_button_cancel.h i_button_ok.h i_button_read.h icons.h i_edittrash.h i_exit.h i_fileclose.h i_fileexport.h i_filenew.h i_wizard.h liste.h mynotes.h spreadsheet.h tools_config.h tools_gui.h
CFILES=book.cpp dbentry.cpp db_settings.cpp db_util.cpp fes.cpp icons.cpp liste.cpp main.cpp mynotes.cpp notes.cpp sequence.cpp sequences.cpp spreadsheet.cpp
OFILES=book.o dbentry.o db_settings.o db_util.o fes.o icons.o liste.o main.o mynotes.o notes.o sequence.o sequences.o spreadsheet.o
VERSION=0.1
CXX=g++
CXXFLAGS= -O2 -I/usr/local/include/fox-1.7  -I/home/martin/programs/alt/. -I/home/martin/programs/alt/tools -I/home/martin/programs/alt/shared
LDFLAGS= -lmysqlclient -L/usr/local/lib -lFOX-1.7 -lXext -lX11 -lXcursor -lXrender -lXrandr -lXfixes -lXi -ldl -lpthread -lrt -ljpeg -lpng -ltiff -lz -lbz2 -lm -lGLU -lGL  -L. -Ltools -Lshared


PACKAGE=dbi
DIST=$(PACKAGE)-$(VERSION)

prefix=$(DESTDIR)/usr/local
bindir=$(prefix)/bin
docdir=$(prefix)/share/doc/$(PACKAGE)

all: alt 

alt: $(OFILES) tools/libtools.a shared/libshared.a
	$(CXX) -o alt $(OFILES) $(CXXFLAGS) $(LDFLAGS) tools/libtools.a shared/libshared.a

clean:
	rm -f core* dbi $(OFILES)
	cd tools && make clean
	cd shared && make clean

tools/libtools.a:
	cd tools && make

shared/libshared.a:
	cd shared && make

dist: clean
	rm -rf $(DIST)
	rm -f $(DIST).tar.gz
	mkdir $(DIST)
	cp $(CFILES) $(HFILES) README COPYING INSTALL genmak configure Makefile.in $(DIST)
	find $(DIST) -name "CVS" | xargs rm -rf
	tar cvf $(DIST).tar $(DIST)
	gzip --best $(DIST).tar
	rm -rf $(DIST)

install: dbi
	if ! [ -d $(bindir) ] ; then mkdir -p $(bindir) ; fi
	cp dbi $(bindir)
	if ! [ -d $(docdir) ] ; then mkdir -p $(docdir) ; fi
	cp README COPYING $(docdir)


book.o: book.cpp $(HFILES)
	$(CXX) -o book.o -c book.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

dbentry.o: dbentry.cpp $(HFILES)
	$(CXX) -o dbentry.o -c dbentry.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

db_settings.o: db_settings.cpp $(HFILES)
	$(CXX) -o db_settings.o -c db_settings.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

db_util.o: db_util.cpp $(HFILES)
	$(CXX) -o db_util.o -c db_util.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

fes.o: fes.cpp $(HFILES)
	$(CXX) -o fes.o -c fes.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

icons.o: icons.cpp $(HFILES)
	$(CXX) -o icons.o -c icons.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

liste.o: liste.cpp $(HFILES)
	$(CXX) -o liste.o -c liste.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

main.o: main.cpp $(HFILES)
	$(CXX) -o main.o -c main.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

mynotes.o: mynotes.cpp $(HFILES)
	$(CXX) -o mynotes.o -c mynotes.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

notes.o: notes.cpp $(HFILES)
	$(CXX) -o notes.o -c notes.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

sequence.o: sequence.cpp $(HFILES)
	$(CXX) -o sequence.o -c sequence.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

sequences.o: sequences.cpp $(HFILES)
	$(CXX) -o sequences.o -c sequences.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

spreadsheet.o: spreadsheet.cpp $(HFILES)
	$(CXX) -o spreadsheet.o -c spreadsheet.cpp $(CXXFLAGS) -DLINUX -DVERSION='"0.1"'

