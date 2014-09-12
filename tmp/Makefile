
CFLAGS=-I../src -g -Wall -O3
LDLIBS=
CC=c99
VPATH=../src

RODZO=$(shell which rodzo)
ifeq ($(RODZO),)
  RODZO=../../rodzo/bin/rodzo
endif

NAMES=$(patsubst $(VPATH)/%.c, ./%.o, $(wildcard $(VPATH)/*.c))
  # since wildcard doesn't care about VPATH...

s.c: ../spec/*_spec.c
	$(RODZO) ../spec -o s.c

s: $(NAMES)

spec: s
	time ./s

vspec: s
	valgrind --leak-check=full -v ./s

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.c
	rm -f s

.PHONY: spec vspec clean

