ifeq ($(OS),Windows_NT)
	PLATFORM := windows
	EXE_EXTENSION := .exe
else
	PLATFORM := linux
	EXE_EXTENSION :=
endif

ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
	PLATFORM_TRIPLET := $(PLATFORM)-x64
endif
ifeq ($(PROCESSOR_ARCHITECTURE),x86)
	PLATFORM_TRIPLET := $(PLATFORM)-x86
endif
ifeq ($(PROCESSOR_ARCHITECTURE),arm)
	PLATFORM_TRIPLET := $(PLATFORM)-arm
endif

CC      := gcc
CFLAGS  := -Wall -Wextra -O3
CCFLAGS := -Ilib/vlcsdk/include
LFLAGS  := -Llib/vlcsdk -lvlc

OUT_DIR  := bin/$(PLATFORM_TRIPLET)
OUT_FILE := bvlc$(EXE_EXTENSION)
SRC_DIR  := src

OBJ_DIR  := $(OUT_DIR)/obj

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

.PHONY: run
run: $(OUT_DIR)/$(OUT_FILE)
	$(OUT_DIR)/$(OUT_FILE)

$(OUT_DIR)/$(OUT_FILE): $(OUT_DIR) $(OBJ_DIR) $(OBJ_FILES)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir $(subst /,\,$(OBJ_DIR))

$(OUT_DIR):
	mkdir $(subst /,\,$(OUT_DIR))