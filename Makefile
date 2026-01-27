.PHONY: all compiler domk2 rtl clean

all: compiler domk2 rtl

compiler:
	$(MAKE) -C src/compiler

domk2:
	$(MAKE) -C src/domk2

rtl:
	$(MAKE) -C src/rtl

clean:
	$(MAKE) -C src/compiler clean
	$(MAKE) -C src/domk2 clean
	$(MAKE) -C src/rtl clean
