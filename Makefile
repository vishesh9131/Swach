# Project Makefile for CMake-based snapshot-publisher

# Determine number of cores, fallback to 1 if unknown
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CPUS := $(shell nproc)
else ifeq ($(UNAME_S),Darwin)
    CPUS := $(shell sysctl -n hw.ncpu)
else
    CPUS := 1
endif

# Directories
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
TEST_BIN := $(BUILD_DIR)/tests/test_runner

# CMake flags
CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Debug

.PHONY: all build test clean

all: build

build:
	@echo "🔧 Building project with CMake..."
	@cmake -Who-dev -S . -B $(BUILD_DIR) $(CMAKE_FLAGS)
	@cmake --build $(BUILD_DIR)  -- -j$(CPUS)

test: build
	@echo "🧪 Running tests..."
	@ctest --test-dir $(BUILD_DIR) --output-on-failure

clean:
	@echo "🧹 Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

