export CC := gcc
export INCLUDE := include
export CFLAGS := -g -O2
export TARGET := simple-tcp

export SHELL := /bin/sh

all:
	$(MAKE) -C src

.phony: clean clean_logs

clean:
	$(MAKE) -C src clean
	$(RM) $(TARGET)

clean_logs:
	$(RM) -r logs/