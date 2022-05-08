# Makefile created for ICP assignment, FIT @ BUT 2021/22
#
# Authors: Hung Do 		(xdohun00@stud.fit.vutbr.cz)
#          David Kedra 	(xkedra00@stud.fit.vutbr.cz)

PROJECT_NAME=DiagramEditor
ZIP_NAME=1_xdohun00_xkedra00

TEST_NAME=tests
BUILD_DIR=./build
BIN_DIR=$(BUILD_DIR)/bin
TARGET=$(BIN_DIR)/$(PROJECT_NAME)
TEST_TARGET=$(BIN_DIR)/$(TEST_NAME)

#######################################

.PHONY: build run test doc pack clean

build: $(TARGET)

run: $(TARGET)
	$<

test: $(TEST_TARGET)
	$<

doc: Doxyfile
	doxygen

pack: src/* examples/* README.txt Makefile CMakeLists.txt Doxyfile $(TEST_NAME)
	rm -f *.zip
	zip -r $(ZIP_NAME) $^
	# zip $(ZIP_NAME) doc

clean:
	rm -rf doc *.zip build/

#######################################

$(TARGET): $(BUILD_DIR)/Makefile ./src/*
	make -C$(BUILD_DIR) -j8

$(TEST_TARGET): $(BUILD_DIR)/Makefile ./tests/*.cpp
	make -C$(BUILD_DIR) -j8

$(BUILD_DIR)/Makefile: CMakeLists.txt
	cmake . -B$(BUILD_DIR)
