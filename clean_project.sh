#!/bin/sh

find . -name "*.o" | xargs rm -f
find . -name "*.a" | xargs rm -f
find . -name "Makefile" | xargs rm -f
rm -f alt all.xml generator/generator
( cd shared && rm -f *.cpp *.h *.sql )
