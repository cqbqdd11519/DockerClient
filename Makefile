NAME=libdocker-cpp
LIB=$(NAME).so

INC=-I. -Irapidjson/include
SRC=docker.cpp
OBJ=docker.o

LD_FLAGS=-lcurl

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

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

install: $(LIB)
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install -m 644 $(LIB) $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 docker.h $(DESTDIR)$(PREFIX)/include/
	ldconfig