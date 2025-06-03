# Compiler and flags
CXX        := g++
CXXFLAGS   := -std=c++17 -Wall -I$(INCDIR) -O2
LDFLAGS    := -lgmpxx -lgmp

# Directory layout
SRCDIR     := src
INCDIR     := include
OBJDIR     := obj
BINDIR     := bin

#                          $(SRCDIR)/tool.cpp              \
# Source files (exclude crypta.cpp, tool.cpp, util.cpp)
SRC_ROOT   := $(filter-out $(SRCDIR)/crypta.cpp           \
                         $(SRCDIR)/util.cpp,             \
                  $(wildcard $(SRCDIR)/*.cpp))
SRC_CIPH   := $(wildcard $(SRCDIR)/ciphers/*.cpp)
SRCS       := $(SRC_ROOT) $(SRC_CIPH)

# List of object files, all living in $(OBJDIR)
# Strip off directories and replace .cpp → .o
OBJS       := $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SRCS)))

# Which object files go into each binary
CIPHER_OBJS := $(OBJDIR)/cipher.o    \
               $(OBJDIR)/cipherutils.o \
               $(OBJDIR)/lattice.o  \
               $(OBJDIR)/rsa.o

KEYGEN_OBJS := $(OBJDIR)/keygen.o    \
               $(OBJDIR)/cipherutils.o \
               $(OBJDIR)/lattice.o  \
               $(OBJDIR)/rsa.o

TOOL_OBJS	:= $(OBJDIR)/tool.o		\
			   $(OBJDIR)/cipherutils.o \

.PHONY: all clean

all: dirs $(BINDIR)/cipher $(BINDIR)/keygen $(BINDIR)/tool

# ensure directories exist
dirs:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Link cipher
$(BINDIR)/cipher: $(CIPHER_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Link keygen
$(BINDIR)/keygen: $(KEYGEN_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Link tool
$(BINDIR)/tool: $(TOOL_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Pattern rule: build any .o from a .cpp in src/
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pattern rule: build any .o from a .cpp in src/ciphers/
$(OBJDIR)/%.o: $(SRCDIR)/ciphers/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean out everything
clean:
	rm -rf $(OBJDIR) $(BINDIR)
