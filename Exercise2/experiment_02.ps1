#############################################
#              BUILD & PREPARE              #
#############################################


Push-Location ".."

$script = $PSScriptRoot+"\..\build.ps1"
& $script

#############################################
#                  CONFIG                   #
# Uncomment options if you like to run them #
#############################################

$algo = @(
    'merge',
    'quick'
)

$type = @(
    'sta',
    'mta'
)

$cores = Read-Host "Number of cores to use"

#############################################
#                  EXECUTE                  #
#############################################

foreach ($a in $algo) {
    foreach ($t in $type) {
        foreach ($i in 3..9) {
            $size = [Math]::Pow(10, $i)
            # Write-Host "bin/ParallelSort.exe $size $a $t $cores"
            Start-Process -Wait -NoNewWindow -FilePath "bin/ParallelSort.exe" -RedirectStandardOutput ".\NUL" -ArgumentList "$size $a $t $cores"
        }
    }
}

#############################################
#                  CLEANUP                  #
#############################################

Pop-Location