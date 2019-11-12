NAME=libdocker-cpp
LIB=$(NAME).so

INC=-I. -Irapidjson/include
SRC=docker.cpp
OBJ=docker.o

LD_FLAGS=-lcurl

all: $(LIB)

$(LIB): $(OBJ)
	g++ -shared -o $(LIB) $(OBJ) $(DEPOBJ) $(LD_FLAGS)

%.o: %.cpp
	g++ -g -std=c++11 -fPIC $(INC) -c $< -o $@ $(LD_FLAGS)

test: $(OBJ) test.o
	g++ -g -o test test.o $(OBJ) $(LD_FLAGS)

clean:
	rm -f $(LIB)
	rm -f test
	rm -f *.o

