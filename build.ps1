If(!(test-path -PathType container "bin"))
{
      New-Item -ItemType Directory -Path "bin"
}

If(!(test-path -PathType container "build"))
{
      New-Item -ItemType Directory -Path "build"
}

$GCC_PATH = (Get-Command gcc).Path
$GPP_PATH = (Get-Command g++).Path

if([string]::IsNullOrWhitespace($GCC_PATH)){
      Write-Error -Message "GCC Compiler not found. Please add it to your path!" -Category NotInstalled
      exit
}

if([string]::IsNullOrWhitespace($GPP_PATH)){
      Write-Error -Message "G++ Compiler not found. Please add it to your path!" -Category NotInstalled
      exit
}

$SC=(Join-Path $PSScriptRoot ".").Split(":").Get(1)
$BC=(Join-Path $PSScriptRoot "build").Split(":").Get(1)

cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=$GCC_PATH -DCMAKE_CXX_COMPILER:FILEPATH=$GPP_PATH -SC:$SC -Bc:$BC -G "MinGW Makefiles"
cmake --build build/ --config Release --target all --
