#############################################
#              BUILD & PREPARE              #
#############################################


Push-Location ".."

$script = $PSScriptRoot + "\..\build.ps1"
& $script

#############################################
#                  CONFIG                   #
# Uncomment options if you like to run them #
#############################################

$algo = @(
    'merge',
    'quick'
)
$size = [Math]::Pow(10, 6)
$cores = [Environment]::ProcessorCount

#############################################
#                  EXECUTE                  #
#############################################

foreach ($a in $algo) {
    foreach ($i in 2..$cores) {
        foreach ($c in 1..100) {
            # Write-Host "bin/ParallelSort.exe $size $a mta $i"
            Start-Process -Wait -NoNewWindow -FilePath "bin/ParallelSort.exe" -RedirectStandardOutput ".\NUL" -ArgumentList "$size $a mta $i"   
        }
    }
}

#############################################
#                  CLEANUP                  #
#############################################

Pop-Location