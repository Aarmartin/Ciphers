CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iinclude
SRCDIR = src
CIPHERS = $(SRCDIR)/ciphers

CIPHER = cipher
FCIPHER = fcipher
CRYPTA = crypta
TOOL = tool

PRIMARY_FILES = $(wildcard $(CIPHERS)/*.cpp) $(SRCDIR)/util.cpp

OBJECTS = $(PRIMARY_FILES:.cpp=.o)

all: $(CIPHER) $(FCIPHER) $(CRYPTA) $(TOOL)

$(CIPHER): $(OBJECTS) $(SRCDIR)/cipher.o
	$(CXX) $(CXXFLAGS) -o $(CIPHER) $(OBJECTS) $(SRCDIR)/cipher.o

$(FCIPHER): $(OBJECTS) $(SRCDIR)/fcipher.o
	$(CXX) $(CXXFLAGS) -o $(FCIPHER) $(OBJECTS) $(SRCDIR)/fcipher.o

$(CRYPTA): $(OBJECTS) $(SRCDIR)/crypta.o
	$(CXX) $(CXXFLAGS) -o $(CRYPTA) $(OBJECTS) $(SRCDIR)/crypta.o

$(TOOL): $(OBJECTS) $(SRCDIR)/tool.o
	$(CXX) $(CXXFLAGS) -o $(TOOL) $(OBJECTS) $(SRCDIR)/tool.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o $(CIPHERS)/*.o $(CIPHER) $(FCIPHER) $(CRYPTA) $(TOOL)