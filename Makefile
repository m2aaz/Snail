BUILD_DIR := build
TARGET := sfml_app

.PHONY: all build run

all: build run

build:
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. 
	cd $(BUILD_DIR) && make

run:
	./$(BUILD_DIR)/$(TARGET)

format:
	clang-format -i src/*.cpp src/*.h