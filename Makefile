
NAME=gajeta

default: $(NAME).o

.DEFAULT spec clean:
	$(MAKE) -C tmp/ $@ NAME=$(NAME)

# copy updated version of dep libs into src/
#
upgrade:
	cp -v ../flutil/src/*.[ch] src/

.PHONY: spec clean

