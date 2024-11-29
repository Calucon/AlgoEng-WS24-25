If(!(test-path -PathType container "bin"))
{
      New-Item -ItemType Directory -Path "bin"
}
If(!(test-path -PathType container "build"))
{
      New-Item -ItemType Directory -Path "build"
}

cmake -B build/ -D CMAKE_BUILD_TYPE=Release .
cmake --build build/
