CXX = g++
CXXFLAGS = -O2 -Wall -Werror -Wextra
RCXXFLAGS := `root-config --cflags --libs`

BINDIR = ./bin
SRCDIR = ./src

SRCS = $(wildcard $(SRCDIR)/*.C)
EXES = $(patsubst $(SRCDIR)/%.C,$(BINDIR)/%,$(SRCS))

.PHONY: all submodules clean

all: $(EXES)

$(BINDIR)/%: $(SRCDIR)/%.C
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(RCXXFLAGS) $< -o $@

clean:
	@rm -f $(EXES)
	@rm -f $(BINDIR)/*
