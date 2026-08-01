# Variables
CC_LINUX := gcc
CC_WIN64 := x86_64-w64-mingw32-gcc
CC_WIN32 := i686-w64-mingw32-gcc

# Appended include source directory discovery
CFLAGS   := -Wall -Wextra -O2 -fno-builtin -pthread -Isrc
LDFLAGS  := -lm -pthread
BUILDDIR := build
SRCDIR   := src

# X11, Cairo & ALSA specific flags for Linux
X11_CFLAGS  := $(shell pkg-config --cflags cairo)
# X11, Cairo, ALSA & Xfixes specific flags for Linux
X11_LDFLAGS := -lX11 -lXext -lXfixes -lasound $(shell pkg-config --libs cairo)


# Windows Specific Linker Flags (Added winmm for waveOut API)
WIN_LDFLAGS := -lgdi32 -lwinmm -static

default: help

all: clean linux win64 win32 run-linux

SRCS := $(wildcard $(SRCDIR)/*.c)

linux:
	mkdir -p $(BUILDDIR)
	$(CC_LINUX) $(CFLAGS) $(X11_CFLAGS) $(SRCS) -o $(BUILDDIR)/BF.x86_64.elf $(LDFLAGS) $(X11_LDFLAGS)

win64:
	mkdir -p $(BUILDDIR)
	$(CC_WIN64) $(CFLAGS) $(SRCS) -o $(BUILDDIR)/BF.win64.exe $(LDFLAGS) $(WIN_LDFLAGS)

win32:
	mkdir -p $(BUILDDIR)
	$(CC_WIN32) $(CFLAGS) $(SRCS) -o $(BUILDDIR)/BF.win32.exe $(LDFLAGS) $(WIN_LDFLAGS)

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
	@echo "  clean       - Remove build artifacts"

.PHONY: all clean linux win64 win32 run-linux run-win64 run-win32 run-wine64 run-wine32 help
