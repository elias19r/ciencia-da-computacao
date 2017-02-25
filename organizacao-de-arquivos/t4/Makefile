# Settings
# --------
#MUTE = @
PROGRAM  = t4
#INPUT    = < test/in/01.in
#OUTPUT   = > test/out/my01.out

# Compiler and options
# --------------------
CC     = gcc
CFLAGS = -Wall -g

# File extensions
# ---------------
SRCEXT = .c
HEADXT = .h
OBJEXT = .o

# Directories
# -----------
BIN   = ./bin
BUILD = ./build
CODE  = ./code
FILE  = ./file
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

LIB_SOURCES := $(wildcard $(SOURCE)/lib/*$(SRCEXT))
LIB_OBJECTS := $(LIB_SOURCES:$(SOURCE)/lib/%$(SRCEXT)=$(BUILD)/lib/%$(OBJEXT))

MODEL_SOURCES := $(wildcard $(SOURCE)/model/*$(SRCEXT))
MODEL_OBJECTS := $(MODEL_SOURCES:$(SOURCE)/model/%$(SRCEXT)=$(BUILD)/model/%$(OBJEXT))

VIEW_SOURCES := $(wildcard $(SOURCE)/view/*$(SRCEXT))
VIEW_OBJECTS := $(VIEW_SOURCES:$(SOURCE)/view/%$(SRCEXT)=$(BUILD)/view/%$(OBJEXT))

CONTROLLER_SOURCES := $(wildcard $(SOURCE)/controller/*$(SRCEXT))
CONTROLLER_OBJECTS := $(CONTROLLER_SOURCES:$(SOURCE)/controller/%$(SRCEXT)=$(BUILD)/controller/%$(OBJEXT))

ALL_OBJECTS = $(CORE_OBJECTS) $(LIB_OBJECTS) $(MODEL_OBJECTS) $(VIEW_OBJECTS) $(CONTROLLER_OBJECTS)

all: $(ALL_OBJECTS)
	$(MUTE)$(CC) -o $(MAIN) $(ALL_OBJECTS)

$(CORE_OBJECTS): $(BUILD)/core/%$(OBJEXT) : $(SOURCE)/core/%$(SRCEXT)
	$(MUTE)$(CC) -c $< -o $@ $(CFLAGS)

$(LIB_OBJECTS): $(BUILD)/lib/%$(OBJEXT) : $(SOURCE)/lib/%$(SRCEXT)
	$(MUTE)$(CC) -c $< -o $@ $(CFLAGS)

$(MODEL_OBJECTS): $(BUILD)/model/%$(OBJEXT) : $(SOURCE)/model/%$(SRCEXT)
	$(MUTE)$(CC) -c $< -o $@ $(CFLAGS)

$(VIEW_OBJECTS): $(BUILD)/view/%$(OBJEXT) : $(SOURCE)/view/%$(SRCEXT)
	$(MUTE)$(CC) -c $< -o $@ $(CFLAGS)

$(CONTROLLER_OBJECTS): $(BUILD)/controller/%$(OBJEXT) : $(SOURCE)/controller/%$(SRCEXT)
	$(MUTE)$(CC) -c $< -o $@ $(CFLAGS)

clean: clear_files
	$(MUTE)rm -f *~
	$(MUTE)rm -f $(BIN)/*
	$(MUTE)rm -f $(BUILD)/*/*
	$(MUTE)rm -f $(INCLUDE)/*/*~
	$(MUTE)rm -f $(SOURCE)/*/*~

clear_files:
	$(MUTE)rm -f *.db
	$(MUTE)rm -f *.idx
	$(MUTE)rm -f $(FILE)/data/*
	$(MUTE)rm -f $(FILE)/index/*
	$(MUTE)rm -f $(ZIP)

run: clear_files
	$(MUTE)$(MAIN) $(INPUT) $(OUTPUT)

x: all run

zip: clean
	zip -r $(ZIP) *

memcheck: all
	valgrind --tool=memcheck --leak-check=full $(MAIN) $(INPUT)

debug: all
	gdb
