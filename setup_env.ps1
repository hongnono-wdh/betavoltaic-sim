# setup_env.ps1 -- load Geant4 dataset env vars + PATH before running betasim.
# Source once per new terminal:   . .\setup_env.ps1
# Assumes Geant4 installed at E:\geant4\install (see GOAL). Change $G4Install if different.
# ASCII-only comments on purpose (PS 5.1 reads BOM-less .ps1 as GBK on zh-CN).
param(
    [string]$G4Install = "E:\geant4\install"
)
$ErrorActionPreference = "Stop"

$ps1 = Join-Path $G4Install "bin\geant4.ps1"
$bat = Join-Path $G4Install "bin\geant4.bat"

if (Test-Path $ps1) {
    Write-Host "[setup_env] sourcing $ps1"
    . $ps1
} elseif (Test-Path $bat) {
    Write-Host "[setup_env] geant4.ps1 missing, importing geant4.bat env..."
    cmd /c "`"$bat`" && set" | ForEach-Object {
        if ($_ -match "^(G4[^=]+)=(.*)$") {
            Set-Item -Path "Env:$($matches[1])" -Value $matches[2]
        }
    }
} else {
    Write-Warning "[setup_env] $ps1 / $bat not found -- is Geant4 installed at $G4Install ?"
}

# Add Geant4 bin (DLLs) to PATH so betasim.exe can load them on Windows
$g4bin = Join-Path $G4Install "bin"
if (Test-Path $g4bin) {
    $env:PATH = "$g4bin;$env:PATH"
    Write-Host "[setup_env] added $g4bin to PATH"
}

# Also import MSVC runtime env (handy when building betasim in the same shell)
$vcvars = "F:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if (Test-Path $vcvars) {
    cmd /c "`"$vcvars`" && set" | ForEach-Object {
        if ($_ -match "^(PATH|INCLUDE|LIB|LIBPATH)=(.*)$") {
            Set-Item -Path "Env:$($matches[1])" -Value $matches[2]
        }
    }
}

Write-Host "[setup_env] done. e.g. G4LEDATA=$env:G4LEDATA"
