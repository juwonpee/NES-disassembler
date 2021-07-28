CC = g++
CC_INCLUDE = src/
CCFLAGS = -pipe -O2
CCFLAGS_DEBUG = -g -pipe -O0

LIBARIES = -lsfml-graphics -lsfml-window -lsfml-system -lpthread

SRC_DIR = src/
OBJECT_DIR = build/object_files/
OUTPUT = build/NES-disassembler
OUTPUT_DEBUG = build/NES-disassembler
MODULES_DIR = $(wildcard $(SRC_DIR)*.cpp)
MODULES = $(notdir $(MODULES_DIR))
OBJECTS = $(wildcard $(OBJECT_DIR)*.o)



all: $(MODULES)
	$(CC) $(OBJECTS) -o $(OUTPUT_DEBUG) $(LIBARIES)

%.cpp:
	$(CC) $(CCFLAGS_DEBUG) -I$(CC_INCLUDE) -c $(SRC_DIR)$@ -o $(OBJECT_DIR)$(addsuffix .o,$(basename $@))

clear:
	rm build/object_files/*