# Algorithm Engineering

Patrick Koston & Simon Schwitz

## Requirements

- `cmake` version `v3.22` or newer
- `gcc` version `v13` or newer
  - `v12` should still work but C23 standard is only experimental

### Windows

- `gcc` supporting the `MinGW Makefiles` Generator

### Linux

- `gcc` supporting the `Unix Makefiles` Generator

### MacOS

- We don't own a Mac, it's way to expernsive
- It is up to the user to find this out

Therefore MacOS builds are not supported from our side

## Build the project

```sh
# Linux
./build.sh

# Windows
./build.ps1

# MacOS
echo "Not supported"
```

You can find the binaries in the `bin/` folder after compilation

## Development Environment

- Visual Studio Code
  - with CMake Extension _(optional)_

## Code Sources

The code for a particular assignment can be found in the corresponding assignment folder  
(e.g. `Exercise1/src`).

### Libs

Code that (may) be shared between multiple assignments can be found in the `lib/` directory.  
There are no external libraries required to build the code.

#### IO

> Source: `lib/IO`  

This library contains everything that is necessary for FileIO.

- FileReader
- FileWriter

#### Sort

> Source: `lib/Sort`

This library implements the various sorting algorithms required by different assignments

- IntroSort
- QuickSort (classic and parallel)
- MergeSort (classic and parallel)

#### Utils

> Source: `lib/Utils`

This library implements various helper classes and functions that are used for different assignments

- NumberGenerator (64bit integer)
- NumberGenerator32 (32bit integer)
