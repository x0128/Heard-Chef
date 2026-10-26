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

Enter-VsShell

if (-not (Test-Path "build/$Preset/build.ninja")) {
    cmake --preset $Preset
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

cmake --build --preset $Preset
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

if ($Run) {
    Start-Process -FilePath "build/$Preset/bin/game.exe" -WorkingDirectory "build/$Preset/bin"
}
exit 0
