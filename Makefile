CC = c++

CXXFLAGS = -std=c++17
# CXXFLAGS += -Ilib/mayGL/include
LDFLAGS = 
#LDFLAGS = -Llib/mayCL/lib/static/ -lmayCL -framework OpenCL
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
# cd lib/mayCL/ && make
	
$(PROGRAM): $(OBJ)
	$(CC) -o $(BIN)/$(PROGRAM) $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)