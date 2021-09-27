.PHONY: all linux windows clean re

UNAME := $(shell uname -s)

all:
ifeq ($(UNAME), Linux)
	$(MAKE) -f Makefile.linux all
else
	$(MAKE) -f Makefile.windows all
endif


linux:
	$(MAKE) -f Makefile.linux all


windows:
	$(MAKE) -f Makefile.windows all


clean:
ifeq ($(UNAME), Linux)
	$(MAKE) -f Makefile.linux clean
else
	$(MAKE) -f Makefile.windows clean
endif

re: clean all
