
NAME=gajeta

default: $(NAME).o

.DEFAULT spec clean:
	$(MAKE) -C tmp/ $@ NAME=$(NAME)

# copy updated version of dep libs into src/
#
stamp:
	cd $(REP) && git log -n 1 | sed 's/^/\/\//' >> ../$(NAME)/$(FIL)
upgrade:
	cp -v ../flutil/src/fluti[lm].[ch] src/
	find src/fluti* -exec $(MAKE) stamp REP=../flutil FIL={} \;

.PHONY: spec clean

