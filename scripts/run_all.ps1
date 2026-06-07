# run_all.ps1 -- golden test one-shot: build -> run 3 structures -> plot -> self-check.
# Prereq: betasim.exe built, and Geant4 env loaded (. .\setup_env.ps1).
# Usage:  .\scripts\run_all.ps1 [-Build]
# ASCII-only comments on purpose (PS 5.1 reads BOM-less .ps1 as GBK on zh-CN).
param(
    [switch]$Build,
    [string]$BuildDir = "E:\code\geant4\build",
    [string]$Root = "E:\code\geant4"
)
$ErrorActionPreference = "Stop"
Set-Location $Root

if ($Build) {
    Write-Host "=== build betasim ===" -ForegroundColor Cyan
    cmake -S $Root -B $BuildDir -G "Ninja" -DCMAKE_BUILD_TYPE=Release
    if ($LASTEXITCODE -eq 0) { cmake --build $BuildDir }
}

$betasim = Join-Path $BuildDir "betasim.exe"
if (-not (Test-Path $betasim)) { throw "betasim.exe not found at $betasim; build first with -Build" }

New-Item -ItemType Directory -Force -Path (Join-Path $Root "data")    | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $Root "figures") | Out-Null

$testBeta = Join-Path $BuildDir "test_beta.exe"
if (Test-Path $testBeta) {
    Write-Host "=== beta-spectrum self-check ===" -ForegroundColor Cyan
    & $testBeta
}

foreach ($m in @("golden_tsc.mac","golden_tnt.mac","golden_etnt.mac")) {
    $path = Join-Path $Root "macros\$m"
    Write-Host "=== run $m ===" -ForegroundColor Cyan
    & $betasim $path
    if ($LASTEXITCODE -ne 0) { throw "run $m failed" }
}

Write-Host "=== plot ===" -ForegroundColor Cyan
python (Join-Path $Root "analysis\plot.py") --data-dir (Join-Path $Root "data") --out-dir (Join-Path $Root "figures")

Write-Host "=== DONE. figures in figures\, data in data\ ===" -ForegroundColor Green
