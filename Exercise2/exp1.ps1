$algo = @(
    'merge',
    'quick'
)
$size = [Math]::Pow(10, 6)
$cores = [Environment]::ProcessorCount

foreach ($a in $algo) {
    Write-Host "`n------------------------- $a -------------------------`n"
    foreach ($i in 2..$cores) {
        Write-Host "`n--------------------------------------------------`n"
        Write-Host "../bin/ParallelSort.exe $size $a mta $i"
        Start-Process -Wait -NoNewWindow -FilePath "../bin/ParallelSort.exe" -ArgumentList "$size $a mta $i"
    }
}