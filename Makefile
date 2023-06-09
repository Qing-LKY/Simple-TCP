export CC := gcc
export INCLUDE := include
export CFLAGS := -g -O2
export LDFLAGS := -lpthread
export TARGET := simple-tcp

export SHELL := /bin/sh

all:
	mkdir -p logs/
	$(MAKE) -C src

.phony: clean clean_logs static

static:
	mkdir -p logs/
	$(MAKE) -C src static

clean:
	$(MAKE) -C src clean
	$(RM) $(TARGET) $(TARGET)-static

clean_logs:
	$(RM) logs/*