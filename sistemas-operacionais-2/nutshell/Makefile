# Makefile for nutshell

# Directions:
#
# make              builds everything
# make clean        clean up the mess
# make dist         pack files to distribute
# make distcheck    make dist and check package
# make distclean    restore to original state
# make install      install the libraries
# make uninstall    uninstall the libraries
# make test         perform all runtime tests
# make extras       show targes in Makefile.extras

PROJECT = nutshell

bin = nutshell

# For each item in $(bin), name the objects needed to build it

nutshell_obj = main.o 

# For each item in $(bin), name the libraries to which it shall be linked

nutshell_lib = libnutshell

# Name your project's libraries

lib = libnutshell

# For each item in $(lib), name the objects needed to build it

libnutshell_obj = parser.o utils.o

# For each item in $(lib), name the headers

libnutshell_h = nutshell.h

# Name your project's auxuliary binaries (not to be installed)

aux_bin = example

# For each item in $(aux_bin), name the objects needed to build it

example_obj = example.o 

# For each item in $(aux_bin), name the libraries to which it shall be linked

example_lib = libnutshell

# Extra files to go in the distribution

EXTRA_DIST = Makefile Makefile.extras

# Installation prefix

PREFIX=./local

#
# PLEASE, DO NOT MODIFY BELLOW THIS LINE
#

# Customize the building environemnt

CC = gcc
CPP_FLAGS = -I. -Wall -Werror -std=c89 --pedantic-errors -D_POSIX_C_SOURCE=200112L 
C_FLAGS = 
LD_FLAGS = -L.
MAKE = make
AR = ar

# Handy variables

lib_static = $(lib:%=%.a)
lib_shared = $(lib:%=%.so)
lib_all = $(lib_static) $(lib_shared)

all_lib_obj = $(foreach i, $(lib:%=%_obj), $($i))
all_lib_h = $(foreach i, $(lib:%=%_h), $($i))
all_bin_obj = $(foreach i, $(bin:%=%_obj), $($i))
all_aux_bin_obj = $(foreach i, $(aux_bin:%=%_obj), $($i))

# Build everything

all: $(lib_static) $(lib_shared) $(bin) $(aux_bin) 

fix:
	export LD_LIBRARY_PATH=.

run:
	clear
	./$(bin)

static: $(lib_static) $(bin:%=%-static) $(aux_bin:%=%-static) 

.SECONDEXPANSION:

# Automaic rules for building libraries

$(lib_static) : %.a : $$($$*_obj)
	$(AR) rcs $@ $^

$(lib_shared) : %.so : $$($$*_obj:.o=-pic.o)
	$(CC) -shared $(LD_FLAGS) $(LDFLAGS) $^ -o $@

# Automatic rules for building binaires

$(bin) $(aux_bin): % : $$($$*_obj)  $(lib_all)
	$(CC) $(LD_FLAGS) $(LDFLAGS) $(filter %.o, $^)  $(patsubst lib%, -l%, $($*_lib)) -o $@

$(bin:%=%-static) $(aux_bin:%=%-static): %-static : $$($$*_obj)  $(lib_static)
	$(CC) $(LD_FLAGS) $(LDFLAGS) $(filter %.o, $^) -Wl,-Bstatic $(patsubst lib%, -l%, $($*_lib)) -Wl,-Bdynamic -o $@ 

# Automatic sufix-rules

%.o : %.c
	$(CC) $(CPP_FLAGS) $(CPPFLAGS) $(C_FLAGS) $(CFLAGS) -c  $< -o $@

%-pic.o : %.c
	$(CC) $(CPP_FLAGS) $(CPPFLAGS) $(C_FLAGS) $(CFLAGS) -c  -fPIC $< -o $@

# Atuomatic header dependences

all_bin_d = $(all_bin_obj:%.o=%.d)
all_aux_bin_d = $(all_aux_bin_obj:%.o=%.d)
all_lib_static_d = $(all_lib_obj:%.o=%.d)
all_lib_shared_d = $(all_lib_obj:%.o=%-pic.d)

%.d : %.c
	$(CC) $(CPP_FLAGS) $(CPPFLAGS) -MM -MT '$(<:.c=.o) $@' $< > $@

%-pic.d : %.c
	$(CC) $(CPP_FLAGS) $(CPPFLAGS) -MM -MT '$(<:.c=-pic.o) $@' $< > $@

NO_D := 
NO_D := $(or $(NO_D), $(findstring clean,$(MAKECMDGOALS)))
NO_D := $(or $(NO_D), $(findstring extras,$(MAKECMDGOALS)))
NO_D := $(or $(NO_D), $(findstring uninstall,$(MAKECMDGOALS)))
ifeq (,$(NO_D))
include $(all_bin_d) $(all_aux_bin_d) $(all_static_lib_d) $(all_lib_shared_d)
endif

# Housekeeping

EXTRA_GARBAGE = *~ \#*

.PHONY: clean

clean: 
	rm -f *.o *.d $(lib_static) $(lib_shared) $(bin) $(aux_bin) $(bin:%=%-static) $(aux_bin:%=%-static)
	rm -f $(EXTRA_GARBAGE)
	rm -rf $(PROJECT) $(PROJECT).tgz

# Distribution

all_sources = $(all_bin_obj:%.o=%.c) $(all_aux_bin_obj:%.o=%.c) $(all_lib_obj:%.o=%.c) 

all_headers := $(shell $(CC) $(CPP_FLAGS) $(CPPFLAGS) -MM -MT " " $(all_sources)) 
all_headers := $(filter %.h, $(all_headers))
all_headers := $(sort $(all_headers)) 

dist: $(all_sources) $(all_headers) $(EXTRA_DIST)
	rm -rf $(PROJECT)
	mkdir $(PROJECT)
	cp $^ $(PROJECT)
	tar zcvf $(PROJECT).tgz $(PROJECT)

distcheck: dist
	if $(MAKE) -C $(PROJECT) ; then echo "\n$(PROJECT).tgz ready for distribution"; \
	else echo "\nSomething wrong with $(PROJECT).tgz"; fi; echo 

# Install, uninstall

install : $(lib_all) $(bin) $(all_lib_h)
	install -d $(PREFIX)/lib
	install -d $(PREFIX)/include
	install -d $(PREFIX)/bin
	cp $(lib_all) $(PREFIX)/lib/
	cp $(all_lib_h) $(PREFIX)/include/
	cp $(bin) $(PREFIX)/bin/
	for i in $(bin:%=%-static); do if test -f $$i ; then cp $$i $(PREFIX)/bin; fi; done

uninstall: 
	for i in $(lib_all) ; do rm -f $(PREFIX)/lib/$$i; done
	for i in $(all_lib_h) ; do rm  -f $(PREFIX)/include/$$i; done
	for i in $(bin) ; do rm -f $(PREFIX)/bin/$$i; done
	for i in $(bin) ; do rm -f $(PREFIX)/bin/$$i-static; done

# Tests

test: all
	$(MAKE) -f Makefile.tests $(filter-out test, $(MAKECMDGOALS))

# Extras

export

.DEFAULT:
	$(MAKE) -f Makefile.extras $(MAKECMDGOALS)

memcheck:
	valgrind --tool=memcheck --leak-check=full ./$(bin)

