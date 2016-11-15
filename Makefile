# Compiler options
CC := g++
CFLAGS := -std=c++11 -g
#CFLAGS := -Wall -Wextra -pedantic

# Directories structure
OBJDIR := obj
SRCDIR := src
INCDIR := include

# Directories for the given libraries
LIBDIR := lib/include
LIB_SRC_DIR := lib

# Files to be compiled
OBJS := $(addprefix $(OBJDIR)/,tools.o InvFile.o boolean.o)

.DEFAULT_GOAL := default

# Default target
.PHONY: default
default: $(OBJS) main | $(OBJDIR)/stem.o
	@echo Building comp4133
	@$(CC) -o comp4133 $(OBJS) $(OBJDIR)/stem.o $(OBJDIR)/main.o

$(OBJDIR)/%.o:  $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo Building $*.o
	@$(CC) -isystem $(LIBDIR) -I $(INCDIR) -c $(CFLAGS) $(SRCDIR)/$*.cpp -o $(OBJDIR)/$*.o


#The given stem file
$(OBJDIR)/stem.o:
	@echo Building stem.o
	@$(CC) -isystem $(LIBDIR) -c $(CFLAGS) $(LIB_SRC_DIR)/stem.cpp -o $(OBJDIR)/stem.o

$(OBJDIR):
	@echo Create $(OBJDIR) directory
	@test -d $(OBJDIR) || mkdir $(OBJDIR)

# Build test case, needs Google Test library
test: $(OBJS) $(OBJDIR)/stem.o main_test $(OBJDIR)/test_case.o
	@echo Building comp4133 and test cases, this creates a dependency on Google Test library.
	@$(CC) -o comp4133 $(OBJS) $(OBJDIR)/stem.o $(CFLAGS) $(OBJDIR)/test_case.o $(OBJDIR)/main.o -lgtest -lpthread

$(OBJDIR)/test_case.o: $(OBJDIR) $(SRCDIR)/test_case.cpp
	@echo Building test_case.o
	@$(CC) -isystem $(LIBDIR) -I $(INCDIR) -c $(CFLAGS) $(SRCDIR)/test_case.cpp -o $(OBJDIR)/test_case.o

# Main program with and without GTEST
main:
	@$(CC) -isystem $(LIBDIR) -I $(INCDIR) -c $(CFLAGS) $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o

main_test:
	@$(CC) -isystem $(LIBDIR) -I $(INCDIR) -c $(CFLAGS) $(SRCDIR)/main.cpp -o $(OBJDIR)/main.o -DRUN_GTEST

.PHONY: clean
clean:
	@echo Clean up all object files
	@rm -rf $(OBJDIR)
	@echo Clean up executable
	@rm -f comp4133 comp4133.exe
