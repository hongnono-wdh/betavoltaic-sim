# install_vs.ps1 — 给已装的 VS2022 Community 静默添加 C++ 组件。
# 根因(已由 dd_installer 日志确认):`--wait` 不是 setup.exe modify 的合法参数 → 退出码 87。
# 正确做法:不传 --wait;用 ProcessStartInfo + WaitForExit() 等待 GUI 安装器;再轮询确认 cl.exe。
# 本脚本被提权后运行;参数用自加引号的单字符串以兼容带空格路径。
$ErrorActionPreference = "Continue"

$setup  = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\setup.exe"
$vsPath = "F:\Program Files\Microsoft Visual Studio\2022\Community"
$msvcDir = "F:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC"
$log    = "E:\code\geant4\build\vs_install_log.txt"
$exitf  = "E:\code\geant4\build\vs_install_exit.txt"

New-Item -ItemType Directory -Force -Path (Split-Path $log) | Out-Null
Remove-Item $exitf -ErrorAction SilentlyContinue

$arguments = 'modify --installPath "' + $vsPath + '"' +
    ' --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64' +
    ' --add Microsoft.VisualStudio.Component.Windows11SDK.22621' +
    ' --add Microsoft.VisualStudio.Component.VC.CMake.Project' +
    ' --quiet --norestart'

"==== start $(Get-Date) ====" | Out-File -FilePath $log -Encoding utf8
"args: $arguments"            | Out-File -FilePath $log -Append -Encoding utf8

$psi = New-Object System.Diagnostics.ProcessStartInfo
$psi.FileName = $setup
$psi.Arguments = $arguments
$psi.UseShellExecute = $false
$psi.CreateNoWindow = $true

$rc = -999
try {
    $proc = [System.Diagnostics.Process]::Start($psi)
    $proc.WaitForExit()
    $rc = $proc.ExitCode
    "setup.exe returned $rc at $(Get-Date)" | Out-File -FilePath $log -Append -Encoding utf8
} catch {
    "EXCEPTION: $($_.Exception.Message)" | Out-File -FilePath $log -Append -Encoding utf8
}

# 轮询兜底:VS 安装器可能自更新后重启子进程；等 cl.exe 真正出现(最多 ~30 min)。
$deadline = (Get-Date).AddMinutes(30)
while ((Get-Date) -lt $deadline) {
    if (Test-Path $msvcDir) {
        $hasCl = Get-ChildItem $msvcDir -Recurse -Filter "cl.exe" -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($hasCl) { "cl.exe found: $($hasCl.FullName)" | Out-File $log -Append -Encoding utf8; break }
    }
    $running = Get-Process -Name "setup","vs_installer","vs_installerservice" -ErrorAction SilentlyContinue
    if (-not $running -and $rc -ne 0 -and $rc -ne -999) { break } # setup 已退出且非成功，无需再等
    Start-Sleep -Seconds 15
}

$ok = Test-Path $msvcDir
"==== done rc=$rc msvc_present=$ok at $(Get-Date) ====" | Out-File -FilePath $log -Append -Encoding utf8
"EXITCODE=$rc;MSVC=$ok" | Out-File -FilePath $exitf -Encoding ascii
