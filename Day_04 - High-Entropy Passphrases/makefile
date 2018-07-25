CCX=g++
CXXFLAGS= -g -Wall

DEP_FLAGS=-MMD -MP

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
DEP=$(SRC:.cpp=.d)

CXXFLAGS+=$(DEP_FLAGS)

TARG=Solution


all: $(TARG) 

$(TARG): $(OBJ) 
	$(CCX) $^ $(LFLAGS) -o $@


.PHONY: clean

clean:
	rm $(OBJ) $(TARG) $(DEP)

-include $(DEP)
