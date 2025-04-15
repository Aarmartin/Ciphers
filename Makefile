CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iinclude
SRCDIR = src
CIPHERS = $(SRCDIR)/ciphers

CIPHER = cipher
FCIPHER = fcipher
BRUTE = bruteforce
FREQUENCY = freq
CAESAR_CRYPTA = ccrypta
VIGENERE_CRYPTA = vcrypta
MTABLE = mtable

PRIMARY_FILES = $(wildcard $(CIPHERS)/*.cpp) $(SRCDIR)/util.cpp

OBJECTS = $(PRIMARY_FILES:.cpp=.o)

all: $(CIPHER) $(FCIPHER) $(BRUTE) $(FREQUENCY) $(CAESAR_CRYPTA) $(MTABLE) $(VIGENERE_CRYPTA)

$(CIPHER): $(OBJECTS) $(SRCDIR)/cipher.o
	$(CXX) $(CXXFLAGS) -o $(CIPHER) $(OBJECTS) $(SRCDIR)/cipher.o

$(FCIPHER): $(OBJECTS) $(SRCDIR)/fcipher.o
	$(CXX) $(CXXFLAGS) -o $(FCIPHER) $(OBJECTS) $(SRCDIR)/fcipher.o

$(BRUTE): $(OBJECTS) $(SRCDIR)/bruteforce.o
	$(CXX) $(CXXFLAGS) -o $(BRUTE) $(OBJECTS) $(SRCDIR)/bruteforce.o

$(FREQUENCY): $(OBJECTS) $(SRCDIR)/freq.o
	$(CXX) $(CXXFLAGS) -o $(FREQUENCY) $(OBJECTS) $(SRCDIR)/freq.o

$(CAESAR_CRYPTA): $(OBJECTS) $(SRCDIR)/ccrypta.o
	$(CXX) $(CXXFLAGS) -o $(CAESAR_CRYPTA) $(OBJECTS) $(SRCDIR)/ccrypta.o

$(VIGENERE_CRYPTA): $(OBJECTS) $(SRCDIR)/vcrypta.o
	$(CXX) $(CXXFLAGS) -o $(VIGENERE_CRYPTA) $(OBJECTS) $(SRCDIR)/vcrypta.o

$(MTABLE): $(OBJECTS) $(SRCDIR)/mtable.o
	$(CXX) $(CXXFLAGS) -o $(MTABLE) $(OBJECTS) $(SRCDIR)/mtable.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o $(CIPHERS)/*.o $(CIPHER) $(FCIPHER) $(BRUTE) $(FREQUENCY) $(CAESAR_CRYPTA) $(VIGENERE_CRYPTA) $(MTABLE)