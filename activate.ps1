$ErrorActionPreference = 'Stop'

$msys2Root = Join-Path $env:LOCALAPPDATA 'Programs\MSYS2'
$mingwPrefix = Join-Path $msys2Root 'ucrt64'
$mingwBin = Join-Path $mingwPrefix 'bin'

foreach ($tool in @('g++.exe', 'cmake.exe', 'ninja.exe', 'gdb.exe')) {
    if (-not (Test-Path -LiteralPath (Join-Path $mingwBin $tool))) {
        throw "Outil manquant : $mingwBin\$tool"
    }
}

$env:MSYS2_ROOT = $msys2Root
$env:MSYSTEM = 'UCRT64'
$env:MINGW_PREFIX = $mingwPrefix
$env:CMAKE_PREFIX_PATH = if ($env:CMAKE_PREFIX_PATH) {
    "$mingwPrefix;$env:CMAKE_PREFIX_PATH"
} else {
    $mingwPrefix
}
$env:PATH = "$mingwBin;$env:PATH"

Set-Location -LiteralPath $PSScriptRoot
Write-Host "Environnement C++ UCRT64 activé (GCC, CMake, Ninja, GDB, Raylib)." -ForegroundColor Green
Write-Host 'Compilation : cmake -S . -B build -G Ninja; cmake --build build'
