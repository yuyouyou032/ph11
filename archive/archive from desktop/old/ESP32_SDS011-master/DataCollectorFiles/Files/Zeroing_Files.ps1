$folder="C:\inetpub\wwwroot\DataCollector\Files\Sensor*"

Get-Childitem $folder | foreach {if ($_.LastWriteTime -lt (Get-Date).AddMinutes((-15))) {Set-Content -Path $_ -Value '0'}}



