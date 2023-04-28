export CC := gcc
export INCLUDE := include
export CFLAGS := -g -O2
export TARGET := simple-tcp

export SHELL := /bin/sh

all:
	$(MAKE) -C src

.phony: clean clean_logs static

static:
	$(MAKE) -C src static

clean:
	$(MAKE) -C src clean
	$(RM) $(TARGET) $(TARGET)-static

clean_logs:
	$(RM) -r logs/