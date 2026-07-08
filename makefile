# Compiler
CC = em++

# Project settings
TARGET = build/index.html
SRC_DIR = src
OBJ_DIR = build/obj
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
RAYLIB_SRC_DIR = /home/stream/raylib/src
RAYLIB_LIB = /home/stream/raylib/build/raylib/libraylib.a

# Flags
# -Os: Optimize for size (ideal for web)
# -s USE_GLFW=3: Raylib web requirement
# -s ASYNCIFY: Needed for complex game loops or web features
# --shell-file: Your custom HTML wrapper
CFLAGS = -Os -Wall -DPLATFORM_WEB -I$(RAYLIB_SRC_DIR)
LDFLAGS = $(RAYLIB_LIB) -s USE_GLFW=3 -s ASYNCIFY -s INITIAL_MEMORY=64MB --shell-file shell.html

# Ensure the build directory exists
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) index.js index.wasm index.data