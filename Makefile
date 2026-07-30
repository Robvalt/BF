# Variables
CC_LINUX := gcc
CC_WIN64 := x86_64-w64-mingw32-gcc
CC_WIN32 := i686-w64-mingw32-gcc

CFLAGS   := -Wall -Wextra -O2 -fno-builtin
LDFLAGS  := -lm
BUILDDIR := build
SRCDIR   := src

# X11 & Cairo specific flags for Linux
# pkg-config safely resolves the installation paths for Cairo headers and libraries
X11_CFLAGS  := $(shell pkg-config --cflags cairo)
X11_LDFLAGS := -lX11 -lXext $(shell pkg-config --libs cairo)

# Default target
default: help

all: clean linux win64 win32

# Source discovery
SRCS := $(wildcard $(SRCDIR)/*.c)

# Build rules
linux:
	mkdir -p $(BUILDDIR)
	$(CC_LINUX) $(CFLAGS) $(X11_CFLAGS) $(SRCS) -o $(BUILDDIR)/BF.x86_64.elf $(LDFLAGS) $(X11_LDFLAGS)

win64:
	mkdir -p $(BUILDDIR)
	$(CC_WIN64) $(CFLAGS) $(SRCS) -o $(BUILDDIR)/BF.win64.exe $(LDFLAGS)

win32:
	mkdir -p $(BUILDDIR)
	$(CC_WIN32) $(CFLAGS) $(SRCS) -o $(BUILDDIR)/BF.win32.exe $(LDFLAGS)

run-linux: linux
	./$(BUILDDIR)/BF.x86_64.elf

run-win64: win64
	./$(BUILDDIR)/BF.win64.exe

run-win32: win32
	./$(BUILDDIR)/BF.win32.exe
	
run-wine64: win64
	wine $(BUILDDIR)/BF.win64.exe

run-wine32: win32
	wine $(BUILDDIR)/BF.win32.exe

clean:
	rm -rf $(BUILDDIR)

help:
	@echo "Usage:"
	@echo "  make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  all         - Build for all platforms"
	@echo "  linux       - Build for Linux (x86_64 with X11/Cairo overlay)"
	@echo "  win64       - Build for Windows (x86_64)"
	@echo "  win32       - Build for Windows (i686)"
	@echo "  run-linux   - Run the Linux build"
	@echo "  run-win64   - Run the Windows x86_64 build"
	@echo "  run-win32   - Run the Windows i686 build"
	@echo "  run-wine64  - Run the Windows x86_64 build with Wine"
	@echo "  run-wine32  - Run the Windows i686 build with Wine"
	@echo "  clean       - Remove build artifacts"
	@echo "  help        - Show this message"

.PHONY: all clean linux win64 win32 run-linux run-win64 run-win32 run-wine64 run-wine32 help