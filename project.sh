#!/bin/sh

OPTS=-j4

./configure $*
echo "======== generating tools ========="
( cd tools && make $OPTS )
echo "======== generating generator ====="
( cd generator && make $OPTS )
echo "======== generating project ======="
( cpp -P project.xml -o all.xml )
if ! generator/generator all.xml shared ; then
	exit 1;
fi
( ./configure $* )
echo "======== compiling project ========"
cd shared && make $OPTS && cd .. && rm -f main.o && make $OPTS 

