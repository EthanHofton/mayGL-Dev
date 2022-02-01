CC = c++

CXXFLAGS = -std=c++17
CXXFLAGS += -Ilib/mayGL/include
LDFLAGS = -Llib/mayGL/lib/static/ -lmayGL
LDFLAGS += -Llib/mayGL/lib/glew/ -lGLEW
LDFLAGS += -framework OpenGL
LDFLAGS += -rpath lib/mayGL/lib/SDL2/build/ lib/mayGL/lib/SDL2/build/libSDL2-2.0.dylib
LDFLAGS += -rpath lib/mayGL/lib/assimp/bin/ lib/mayGL/lib/assimp/bin/libassimp.5.2.0.dylib
PROGRAM = out
BIN = bin

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

.PHONY: all clean

all: dirs libs $(PROGRAM)

run: all
	./$(BIN)/$(PROGRAM)

dirs:
	mkdir -p $(BIN)

libs:
	cd lib/mayGL/ && sudo make
	
$(PROGRAM): $(OBJ)
	$(CC) -o $(BIN)/$(PROGRAM) $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)