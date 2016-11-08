CC := g++
OBJDIR := obj
SRCDIR := src
LIBDIR := lib/include
LIB_SRC_DIR := lib
OBJS := $(addprefix $(OBJDIR)/,tools.o main.o)
CFLAGS :=
#CFLAGS := -Wall -Wextra -pedantic

main: $(OBJS) | $(OBJDIR)/stem.o
	@echo Building comp4133
	@$(CC) -o comp4133 $(OBJS) $(OBJDIR)/stem.o

$(OBJDIR)/%.o:  $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo Building $*.o
	@$(CC) -isystem $(LIBDIR) -c $(CFLAGS) $(SRCDIR)/$*.cpp -o $(OBJDIR)/$*.o

#The given stem file
$(OBJDIR)/stem.o:
	@echo Building stem.o
	@$(CC) -isystem $(LIBDIR) -c $(CFLAGS) $(LIB_SRC_DIR)/stem.cpp -o $(OBJDIR)/stem.o

$(OBJDIR):
	@echo Create $(OBJDIR) directory
	@test -d $(OBJDIR) || mkdir $(OBJDIR)

.PHONY: clean
clean:
	@echo Clean up all object files
	@rm -rf $(OBJDIR)
	@echo Clean up executable
	@rm comp4133
