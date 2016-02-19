BIN = ./bin/
SRC = ./src/

OBJS = $(BIN)main.o
TARGET = ./bin/linked_ptr
CXXFLAGS = -O2 -std=c++11

all: bin build

build: $(OBJS) 
	g++ $(OBJS) $(CXXFLAGS) -o $(TARGET)

$(BIN)%.o: $(SRC)%.cpp
	g++ -c $< $(CXXFLAGS) -o $@

bin:
	mkdir -p bin

clean:
	rm -r $(BIN)*
