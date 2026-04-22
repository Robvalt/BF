# BF

## Description
A basic "virus" project im working on, currently has some build targets and a makefile.
Note that this project has <strong>NOT</strong> been tested on : 
* Windows (32bit)
* Windows (64bit)
Or any use of wine.

# Building
The building process is similar to most make-based builds.
The project has 5 build targets and 5 run targets.

## Build process (Linux/Mac) :
```bash
make linux
make run-linux
```

## Build process (Win32) :
```bash
make win32
make run-win32
```

## Build process (Win64) :
```bash
make win64
make run-win64
```

## Build process (Wine32) :
```bash
make win32
make run-wine32
```

## Build process (Linux) :
```bash
make win64
make run-wine64
```

## Build process (Build all without running) :
```bash
make all
```

## Build Targets:
* all
* clear
* linux
* win32
* win64

## Run Targets:
(Note that the run-wine32 and run-wine64 require running linux with wine and winetweaks installed.)
* run-linux
* run-win32
* run-win64
* run-wine32
* run-wine64
