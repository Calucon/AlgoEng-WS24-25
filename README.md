# Algorithm Engineering

Patrick Koston & Simon Schwitz

## Requirements

- `cmake` version `v3.22` or newer
- `gcc` version `v13` or newer
  - `v12` should still work but C23 standard is only experimental

## Build the project

```sh
# Linux
./build.sh

# Windows
./build.ps1
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

#### Sort

> Source: `lib/Sort`

This library implements the various sorting algorithms required by different assignments

#### Utils

> Source: `lib/Utils`

This library implements various helper classes and functions that are used for different assignments
