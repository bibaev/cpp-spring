BIN = ./bin/
SRC = ./src/

OBJS = $(BIN)main.o
TARGET = ./bin/dup_calc
CXXFLAGS = -O2 -std=c++0x -lstdc++fs -lcrypto

all: bin build

build: $(OBJS) 
	g++ $(OBJS) $(CXXFLAGS) -o $(TARGET)

$(BIN)%.o: $(SRC)%.cpp
	g++ -c $< $(CXXFLAGS) -o $@

bin:
	mkdir -p bin

clean:
	rm -r $(BIN)*
