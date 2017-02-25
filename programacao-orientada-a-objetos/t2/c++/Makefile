# Settings
# --------
#MUTE = @
PROGRAM  = t2
#INPUT    = < ../test/in
#OUTPUT   = > ../test/out

# Compiler and options
# --------------------
CC     = g++
CFLAGS = -Wall -g

# File extensions
# ---------------
SRCEXT = .cc
HEADXT = .h
OBJEXT = .o

# Directories
# -----------
BIN   = ./bin
BUILD = ./build
CODE  = ./code
DATA  = ./data
INCLUDE = $(CODE)/include
SOURCE  = $(CODE)/source

# Binary and zip
# --------------
MAIN = $(BIN)/$(PROGRAM)
ZIP  = $(PROGRAM).zip

# Source-codes and object-files
# -----------------------------
CORE_SOURCES := $(wildcard $(SOURCE)/core/*$(SRCEXT))
CORE_OBJECTS := $(CORE_SOURCES:$(SOURCE)/core/%$(SRCEXT)=$(BUILD)/core/%$(OBJEXT))

TYPE_SOURCES := $(wildcard $(SOURCE)/type/*$(SRCEXT))
TYPE_OBJECTS := $(TYPE_SOURCES:$(SOURCE)/type/%$(SRCEXT)=$(BUILD)/type/%$(OBJEXT))

UTIL_SOURCES := $(wildcard $(SOURCE)/util/*$(SRCEXT))
UTIL_OBJECTS := $(UTIL_SOURCES:$(SOURCE)/util/%$(SRCEXT)=$(BUILD)/util/%$(OBJEXT))

ALL_OBJECTS = $(CORE_OBJECTS) $(TYPE_OBJECTS) $(UTIL_OBJECTS)

all: clean $(ALL_OBJECTS)
	$(MUTE)$(CC) -o $(MAIN) $(ALL_OBJECTS)

$(CORE_OBJECTS): $(BUILD)/core/%$(OBJEXT) : $(SOURCE)/core/%$(SRCEXT)
	$(MUTE)$(CC) -c $< -o $@ $(CFLAGS)

$(TYPE_OBJECTS): $(BUILD)/type/%$(OBJEXT) : $(SOURCE)/type/%$(SRCEXT)
	$(MUTE)$(CC) -c $< -o $@ $(CFLAGS)

$(UTIL_OBJECTS): $(BUILD)/util/%$(OBJEXT) : $(SOURCE)/util/%$(SRCEXT)
	$(MUTE)$(CC) -c $< -o $@ $(CFLAGS)

run: clear_files
	$(MUTE)$(MAIN) $(INPUT) $(OUTPUT)

clean: clear_files
	$(MUTE)rm -f *~
	$(MUTE)rm -f $(BIN)/*
	$(MUTE)rm -f $(BUILD)/*/*
	$(MUTE)rm -f $(INCLUDE)/*/*~
	$(MUTE)rm -f $(SOURCE)/*/*~

clear_files:
	$(MUTE)rm -f $(DATA)/*
	$(MUTE)rm -f $(ZIP)

x: all run

zip: clean
	zip -r $(ZIP) *

memcheck: all
	valgrind --tool=memcheck --leak-check=full $(MAIN) $(INPUT)

debug: all
	gdb
