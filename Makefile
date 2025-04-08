# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# SFML paths
SFML_INCLUDE = -IC:\Users\DELL\Documents\libraries\SFML-2.5.1\include
SFML_LIB = -LC:\Users\DELL\Documents\libraries\SFML-2.5.1\lib
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Filesystem library (needed for std::filesystem)
FS_LIB = -lstdc++fs

# Source file
SRC = main.cpp

# Output executable
TARGET = main.exe

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SFML_INCLUDE) -o $(TARGET) $(SRC) $(SFML_LIB) $(SFML_LIBS) $(FS_LIB)

# Clean up build files
clean:
	del $(TARGET)

# Phony targets
.PHONY: all clean