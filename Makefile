NAME=libdocker-cpp
LIB=$(NAME).so

SRC=docker.cpp
OBJ=docker.o

DEPDIR=jsonxx
DEPSRC=$(DEPDIR)/jsonxx.cc
DEPOBJ=jsonxx.o

LD_FLAGS=-lcurl

all: $(LIB)

$(LIB): $(OBJ) $(DEPOBJ)
	g++ -shared -o $(LIB) $(OBJ) $(DEPOBJ) $(LD_FLAGS)

$(OBJ): $(SRC)
	g++ -fPIC -c $< -o $@ $(LD_FLAGS)

$(DEPOBJ): $(DEPSRC)
	g++ -fPIC -c $< -o $@

clean:
	rm -f $(LIB)
	rm -f $(DEPOBJ)
	rm -f $(OBJ)

