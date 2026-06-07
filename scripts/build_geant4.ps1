# build_geant4.ps1 -- configure / build / install Geant4 11.3.2 into E:\geant4\install.
# Prereq: VS C++ tools installed (cl.exe / cmake / ninja available).
# Auto-imports the MSVC x64 dev environment (vcvars64); no need for a Developer prompt.
# Takes 30min-2h; run in background.
# NOTE: ASCII-only comments on purpose -- Windows PowerShell 5.1 reads BOM-less .ps1
#       as the system code page (GBK on zh-CN), which corrupts non-ASCII comments.
param(
    [string]$Src     = "E:\geant4\src",
    [string]$Build   = "E:\geant4\build",
    [string]$Install = "E:\geant4\install",
    [string]$VsPath  = "F:\Program Files\Microsoft Visual Studio\2022\Community"
)
$ErrorActionPreference = "Stop"

# --- Import MSVC x64 dev environment into this PowerShell session ---
$vcvars = Join-Path $VsPath "VC\Auxiliary\Build\vcvars64.bat"
if (-not (Test-Path $vcvars)) { throw "vcvars64.bat not found: $vcvars (C++ tools installed?)" }
Write-Host "[build_geant4] importing MSVC env..." -ForegroundColor Cyan
cmd /c "`"$vcvars`" && set" | ForEach-Object {
    if ($_ -match "^([^=]+)=(.*)$") { Set-Item -Path "Env:$($matches[1])" -Value $matches[2] }
}

foreach ($t in @("cl","cmake","ninja")) {
    $c = Get-Command $t -ErrorAction SilentlyContinue
    if (-not $c) { throw "$t not on PATH (MSVC/CMake/Ninja not ready)" }
    Write-Host "  $t -> $($c.Source)"
}

New-Item -ItemType Directory -Force -Path $Build | Out-Null

# Switching shared->static: drop the stale CMakeCache (keep downloaded datasets in data/ & Externals/)
$cache = Join-Path $Build "CMakeCache.txt"
if (Test-Path $cache) {
    Write-Host "[build_geant4] removing stale CMakeCache (keeping downloaded datasets)..." -ForegroundColor Yellow
    Remove-Item $cache -Force
    Remove-Item (Join-Path $Build "CMakeFiles") -Recurse -Force -ErrorAction SilentlyContinue
}

Write-Host "[build_geant4] CMake configure (STATIC libs to avoid LNK1189)..." -ForegroundColor Cyan
cmake -S $Src -B $Build -G "Ninja" `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_INSTALL_PREFIX="$Install" `
    -DGEANT4_INSTALL_DATA=ON `
    -DGEANT4_USE_QT=OFF `
    -DGEANT4_BUILD_MULTITHREADED=OFF `
    -DGEANT4_USE_SYSTEM_EXPAT=OFF `
    -DBUILD_SHARED_LIBS=OFF `
    -DBUILD_STATIC_LIBS=ON
if ($LASTEXITCODE -ne 0) { throw "CMake configure failed" }

Write-Host "[build_geant4] build + install (long)..." -ForegroundColor Cyan
cmake --build $Build --target install
if ($LASTEXITCODE -ne 0) { throw "build/install failed" }

Write-Host "[build_geant4] DONE. Installed at $Install" -ForegroundColor Green
if (Test-Path (Join-Path $Install "bin\geant4.ps1")) {
    Write-Host "  env script: $Install\bin\geant4.ps1" -ForegroundColor Green
}
