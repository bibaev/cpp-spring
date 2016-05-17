BIN = ./bin/
SRC = ./src/

OBJS = $(BIN)main.o \
	$(BIN)bind_tests.o \
	$(BIN)function_tests.o \
	$(BIN)my_tests.o
TARGET = ./bin/bind_function
CXXFLAGS = -std=c++11 -Wall -Werror -g 

all: bin build

build: $(OBJS) 
	g++ $(OBJS) $(CXXFLAGS) -o $(TARGET)

$(BIN)%.o: $(SRC)%.cpp
	g++ -c $< $(CXXFLAGS) -o $@

bin:
	mkdir -p bin

memcheck: all
	valgrind --tool=memcheck --leak-check=full $(TARGET)

clean:
	rm -r $(BIN)*
