$algo = @(
    'merge',
    'quick'
)

$type = @(
    'sta',
    'mta'
)

$cores = Read-Host "Number of cores to use"

foreach ($a in $algo) {
    Write-Host "`n------------------------- $a -------------------------`n"
    foreach ($t in $type) {
        Write-Host "`n------------------------- $a ----- $t -------------------------`n"
        foreach ($i in 3..9) {
            $size = [Math]::Pow(10, $i)
            Write-Host "`n------------------------- $a ----- $t ----- $i -------------------------`n"
            Write-Host "../bin/ParallelSort.exe $size $a $t $cores"
            Start-Process -Wait -NoNewWindow -FilePath "../bin/ParallelSort.exe" -ArgumentList "$size $a $t $cores"
        }
    }
}