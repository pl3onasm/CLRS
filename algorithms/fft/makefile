# Author: David De Potter
# Date: 2024-08-29

CC = gcc
CFLAGS = -O2 -Wall -pedantic -std=c99
LIBDIRS = ../../lib natlib fftlib
LIBOBJS = $(foreach dir, $(LIBDIRS), $(wildcard $(dir)/*.c))
LIBOBJS := $(patsubst %.c, %.o, $(LIBOBJS))
SRCS := $(wildcard *.c)
BINS = $(patsubst %.c, %.out, $(SRCS))
OBJS = $(patsubst %.c, %.o, $(SRCS))

.PHONY: all clean allclean

all: $(BINS)
	@echo "Completed.\n\nTo run on test input:"
	@echo "$$ ./$(lastword $(BINS)) < tests/<num>.in"
	@chmod +x $(BINS)

$(BINS): %.out: %.o $(LIBOBJS)
	@echo "Building $@ ..."
	@ $(CC) $(CFLAGS) -o $@ $^ -lm

$(OBJS): %.o: %.c
	@echo "Compiling $@ ..."
	@ $(CC) $(CFLAGS) -c $^

$(LIBOBJS): %.o: %.c
	@echo "Compiling $@ ..."
	@ (cd $(dir $@) && $(CC) $(CFLAGS) -c $(notdir $^))
	
clean:
	@echo "Cleaning up working directory ..."
	@rm -f $(BINS) $(OBJS) 

allclean: clean
	@echo "Cleaning up all remaining lib objects ..."
	@rm -f $(LIBOBJS)
