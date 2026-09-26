Nome = hashC

CXX ?= g++

CXXFLAGS += -c -std=c++11 -Wall

all: $(Nome)

$(Nome): $(Nome).o; $(CXX) $< -o $@
%.o: %.cpp; $(CXX) $< -o $@ $(CXXFLAGS)

clean: ; rm -f $(Nome) $(Nome).o
