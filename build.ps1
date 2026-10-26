param([string]$Preset = "debug", [switch]$Run)
$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

function Enter-VsShell {
    if ($env:VSCMD_VER) {
        return
    }
    $VsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $Vs = & $VsWhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    Import-Module "$Vs\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
    Enter-VsDevShell -VsInstallPath $Vs -SkipAutomaticLocation -DevCmdArguments "-arch=x64 -host_arch=x64" | Out-Null
}

function Invoke-Checked([scriptblock]$Command) {
    & $Command
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

$Bin = "build/$Preset/bin"
$HotReload = $Preset -eq "hot-reload"
$Web = $Preset -eq "web"

if ($Web) {
    if (-not $env:EMSDK) {
        $env:EMSDK = "C:\emsdk"
    }
    $env:EMSDK_QUIET = "1"
    $ErrorActionPreference = "Continue"
    . "$env:EMSDK\emsdk_env.ps1" *> $null
    $ErrorActionPreference = "Stop"
} else {
    Enter-VsShell
}

if ($HotReload -and (Get-Process game_hot_reload -ErrorAction SilentlyContinue)) {
    $Lock = "$Bin/game.lock"
    New-Item -ItemType File -Force $Lock | Out-Null
    try {
        cmake --build --preset $Preset --target game
        $Code = $LASTEXITCODE
    } finally {
        Remove-Item $Lock -ErrorAction SilentlyContinue
    }
    exit $Code
}

if (-not (Test-Path "build/$Preset/build.ninja")) {
    Invoke-Checked { cmake --preset $Preset }
}
Invoke-Checked { cmake --build --preset $Preset }

if ($Run -and $Web) {
    Start-Process -FilePath python -ArgumentList "-m", "http.server", "8000" -WorkingDirectory $Bin
    Start-Sleep -Seconds 1
    Start-Process "http://localhost:8000"
} elseif ($Run) {
    $Exe = if ($HotReload) { "game_hot_reload.exe" } else { "game.exe" }
    Start-Process -FilePath "$Bin/$Exe" -WorkingDirectory $Bin
}
exit 0
