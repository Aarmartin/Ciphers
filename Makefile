CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iinclude
SRCDIR = src
CIPHERS = $(SRCDIR)/ciphers

CIPHER = cipher
FCIPHER = fcipher
BRUTE = bruteforce
FREQUENCY = freq
CRYPTA = crypta
MTABLE = mtable
MINVERSE = minverse
FKEY = findkey

PRIMARY_FILES = $(wildcard $(CIPHERS)/*.cpp) $(SRCDIR)/util.cpp

OBJECTS = $(PRIMARY_FILES:.cpp=.o)

all: $(CIPHER) $(FCIPHER) $(BRUTE) $(FREQUENCY) $(MTABLE) $(CRYPTA) $(MINVERSE) $(FKEY)

$(CIPHER): $(OBJECTS) $(SRCDIR)/cipher.o
	$(CXX) $(CXXFLAGS) -o $(CIPHER) $(OBJECTS) $(SRCDIR)/cipher.o

$(FCIPHER): $(OBJECTS) $(SRCDIR)/fcipher.o
	$(CXX) $(CXXFLAGS) -o $(FCIPHER) $(OBJECTS) $(SRCDIR)/fcipher.o

$(BRUTE): $(OBJECTS) $(SRCDIR)/bruteforce.o
	$(CXX) $(CXXFLAGS) -o $(BRUTE) $(OBJECTS) $(SRCDIR)/bruteforce.o

$(FREQUENCY): $(OBJECTS) $(SRCDIR)/freq.o
	$(CXX) $(CXXFLAGS) -o $(FREQUENCY) $(OBJECTS) $(SRCDIR)/freq.o

$(CRYPTA): $(OBJECTS) $(SRCDIR)/crypta.o
	$(CXX) $(CXXFLAGS) -o $(CRYPTA) $(OBJECTS) $(SRCDIR)/crypta.o

$(MTABLE): $(OBJECTS) $(SRCDIR)/mtable.o
	$(CXX) $(CXXFLAGS) -o $(MTABLE) $(OBJECTS) $(SRCDIR)/mtable.o

$(MINVERSE): $(OBJECTS) $(SRCDIR)/minverse.o
	$(CXX) $(CXXFLAGS) -o $(MINVERSE) $(OBJECTS) $(SRCDIR)/minverse.o

$(FKEY): $(OBJECTS) $(SRCDIR)/findkey.o
	$(CXX) $(CXXFLAGS) -o $(FKEY) $(OBJECTS) $(SRCDIR)/findkey.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o $(CIPHERS)/*.o $(CIPHER) $(FCIPHER) $(BRUTE) $(FREQUENCY) $(MTABLE) $(CRYPTA) $(MINVERSE) $(FKEY)