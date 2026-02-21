# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Sources
SRC = main.cpp sphere.cpp vec3.cpp ray.cpp

# Objects (replace .cpp with .o)
OBJ = $(SRC:.cpp=.o)

# Output executable
TARGET = raytracer

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Compile .cpp → .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ) $(TARGET)