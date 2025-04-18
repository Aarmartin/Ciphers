# Setup
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Iinclude -MMD -MP

# Directories
SRCDIR = src
CIPHDIR = $(SRCDIR)/ciphers
OBJDIR = objs
BINDIR = bin

# Objects
LIB_OBJS = $(patsubst $(CIPHDIR)/%.cpp,$(OBJDIR)/ciphers/%.o,$(wildcard $(CIPHDIR)/*.cpp))								# Ciphers
UTIL_OBJS = $(OBJDIR)/util.o																							# Util File
MAIN_OBJS  := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(filter-out $(SRCDIR)/util.cpp,$(wildcard $(SRCDIR)/*.cpp)))	# Main Files

# Binaries
BINNS = cipher fcipher crypta tool
BINS = $(addprefix $(BINDIR)/,$(BINNS))

.PHONY: all clean dirs

all: dirs $(BINS)

# Make Directory
dirs:
	@mkdir -p $(OBJDIR) \
			$(OBJDIR)/ciphers \
			$(BINDIR)

# Link Cipher and Util objects with each Binary
$(BINDIR)/%: $(UTIL_OBJS) $(LIB_OBJS) $(OBJDIR)/%.o | dirs
	@echo "Linking $@"
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build Ciphers
$(OBJDIR)/ciphers/%.o: $(CIPHDIR)/%.cpp | dirs
	@echo "Building $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build Source Objects
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | dirs
	@echo "Building $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Make Clean
clean:
	@echo "Cleaning..."
	rm -rf $(OBJDIR) $(BINDIR)

-include $(OBJDIR)/**/*.d