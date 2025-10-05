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

CC        := gcc
CXX       := g++
CFLAGS    := -Ilib/vlcsdk/include
CXXFLAGS  := -Wall -Wextra -O3 -Ilib/vlcsdk/include
LIBS      := -Llib/vlcsdk/lib -lvlc -lvlccore -lraylib -lglfw3 -lgdi32 -lwinmm

OUT_DIR  := bin/$(PLATFORM_TRIPLET)
OUT_FILE := bvlc$(EXE_EXTENSION)
SRC_DIR  := src

OBJ_DIR  := $(OUT_DIR)/obj

SRC_C_FILES := $(wildcard $(SRC_DIR)/*.c)
SRC_CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_C_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_C_FILES))
OBJ_CPP_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.opp,$(SRC_CPP_FILES))

OBJ_FILES := $(OBJ_C_FILES) $(OBJ_CPP_FILES)

.PHONY: run
run: build
	$(OUT_DIR)/$(OUT_FILE)

.PHONY: build
build: $(OUT_DIR) $(OBJ_DIR) $(OUT_DIR)/$(OUT_FILE)

$(OUT_DIR)/$(OUT_FILE): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)/%.opp: $(SRC_DIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
# OPP NOT RECOGNIZED ?????

$(OBJ_DIR):
	mkdir $(subst /,\,$(OBJ_DIR)) | echo ""

$(OUT_DIR):
	mkdir $(subst /,\,$(OUT_DIR)) | echo ""