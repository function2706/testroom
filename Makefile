#
# Makefile for testroom
#

CC       = $(CROSS_COMPILE)gcc
STRIP    = $(CROSS_COMPILE)strip
CXX      = $(CROSS_COMPILE)g++

CXXFLAGS = -std=c++14 -pthread -lm -D_GNU_SOURCE -Wall -Wtype-limits -g
CXXFLAGS += \
	-Wall                           \
	-Wextra                         \
	-Wundef                         \
	-Wno-unused-parameter           \
	-Wsign-compare                  \
	-Wattributes                    \
	-Wbuiltin-macro-redefined       \
	-Wcast-align                    \
	-Wcomment                       \
	-Wdisabled-optimization         \
	-Wdiv-by-zero                   \
	-Winline                        \
	-Wint-to-pointer-cast           \
	-Wlogical-op                    \
	-Woverflow                      \
	-Wreturn-type                   \
	-Wuninitialized                 \
	-Wunused                        \
	-Wmissing-declarations          \
	-Wshadow                        \

LIBS     = -lpthread -lrt -lncurses
LIBOBJ   =

# Debug for GDB Build
OPTIMIZEOPTION = -Og -DDEBUG=$(DEBUG)

CXXFLAGS += $(OPTIMIZEOPTION) -MMD $(INCLUDES)

LDFLAGS  =

# Target binary file name
TARGETS   = testroom

# Directory to create binary
TARGETDIR = .

# Source code directory
SRCROOT   = .

# Object files place
OBJROOT   = ./obj

SRCDIRS  := $(shell find $(SRCROOT) -type d -not \( -path './stash' \))
SOURCES   = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cc))
OBJECTS   = $(addprefix $(OBJROOT)/, $(SOURCES:.cc=.o))
OBJDIRS   = $(addprefix $(OBJROOT)/, $(SRCDIRS))
DEPENDS   = $(OBJECTS:.o=.d)

all: $(TARGETS)

reflesh: clean all

$(TARGETS): $(OBJECTS)
	$(CXX) -o $(TARGETDIR)/$@ $^ $(LIBOBJ) $(LDFLAGS) $(LIBS) $(CXXFLAGS)

$(OBJROOT)/%.o: $(SRCROOT)/%.cc
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

-include $(DEPENDS)

clean:
	-rm -rf $(TARGETS) $(OBJROOT)

allclean: clean
