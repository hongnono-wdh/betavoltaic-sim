# Geant4 β 能量沉积仿真（复现论文 Fig.2）实现计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 用原生 C++ Geant4 复现论文 Fig.2 —— 模拟 ⁶³Ni β 粒子在 TSC/TNTAs/E-TNTAs 三种结构中的轨迹与能量沉积（EDD/EDR vs 深度），导出 CSV 并用 matplotlib 出图。

**Architecture:** C++ Geant4 应用（Livermore 物理 + ⁶³Ni β 谱源 + 三结构几何 + SteppingAction 打分）将结果写为 CSV；Python/matplotlib 读 CSV 出图。仿真与绘图通过 CSV 解耦。

**Tech Stack:** Geant4 11.3.x（MSVC 编译，装 E 盘）、CMake、Visual Studio 2022（C++ 工作负载）、Python 3.13 + numpy + matplotlib。

**关键路径约定：** 项目根 `E:\code\geant4`；Geant4 装在 `E:\geant4\install`；数据 `data\`；图 `figures\`；源码 `src\`。

---

## 文件结构

```
E:\code\geant4\
  .gitignore
  README.md
  CMakeLists.txt                  # 构建 betasim + test_beta
  setup_env.ps1                   # 设置 G4 数据集环境变量 + PATH（运行前 source）
  scripts\run_all.ps1             # 一键跑三结构 + 出图
  src\
    BetaSpectrum.hh               # 纯 C++ Ni-63 β 谱采样（可单测）
    DetectorConstruction.hh/.cc   # 三结构几何 + 材料
    PhysicsList.hh/.cc            # Livermore EM + cuts
    PrimaryGeneratorAction.hh/.cc # ⁶³Ni β 源
    Analysis.hh/.cc               # 直方图聚合 + CSV 写出（单例）
    EventAction.hh/.cc            # 当前事件 id + 是否记录轨迹
    SteppingAction.hh/.cc         # EDD/EDR + 轨迹打分
    RunAction.hh/.cc              # 运行起止
    ActionInitialization.hh/.cc   # 装配 user actions
    main.cc                       # 入口（CLI: structure nEvents nTraj prefix）
  test\
    test_beta.cc                  # 纯 C++ 单测：β 谱平均能量 ≈17.4 keV
  analysis\
    plot.py                       # 出图：EDD / EDR / 轨迹
    requirements.txt
  data\                           # 运行后生成
  figures\                        # 运行后生成
  build\                          # CMake 构建目录（gitignore）
```

---

## Task 0: 项目脚手架与版本控制

**Files:**
- Create: `E:\code\geant4\.gitignore`
- Create: `E:\code\geant4\README.md`

- [ ] **Step 1: 初始化 git（本地检查点用，无远程）**

Run:
```powershell
cd E:\code\geant4
git init
```
Expected: `Initialized empty Git repository ...`
（若 `git` 未安装：`winget install Git.Git`，重开终端后再试。）

- [ ] **Step 2: 写 `.gitignore`**

```gitignore
build/
data/
figures/
*.exe
*.obj
*.pdb
__pycache__/
.idea/
```

- [ ] **Step 3: 写 `README.md` 骨架**

```markdown
# Betavoltaic Fig.2 Reproduction (Geant4)

复现 Composites Part B 239 (2022) 109952 的 Fig.2：⁶³Ni β 粒子在 TSC/TNTAs/E-TNTAs
中的能量沉积（EDD/EDR）与轨迹。

## 构建与运行
见 `docs/superpowers/plans/`。一键流程：`scripts\run_all.ps1`。
```

- [ ] **Step 4: 首次提交**

```powershell
git add .gitignore README.md docs
git commit -m "chore: scaffold project, add design + plan docs"
```
Expected: 提交成功。

---

## Task 1: 安装 MSVC C++ 工作负载（完整，不精简）

**Files:** 无（环境安装）

> 用户要求：**装完整工作负载，含 ATL/MFC 等可选组件，不要精简**（C 盘 109GB 充足）。

- [ ] **Step 1: 先查准 VS 实际安装路径**

Run:
```powershell
$VSPATH = & "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
$VSPATH
```
Expected: 打印实际安装路径（如 `C:\Program Files\Microsoft Visual Studio\2022\Community`）。下一步用这个 `$VSPATH`。

- [ ] **Step 2: 用 VS Installer 完整补装 C++ 工作负载（含推荐+可选，ATL/MFC 全上）**

Run:
```powershell
& "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vs_installer.exe" modify `
  --installPath "$VSPATH" `
  --add Microsoft.VisualStudio.Workload.NativeDesktop `
  --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
  --add Microsoft.VisualStudio.Component.VC.ATL `
  --add Microsoft.VisualStudio.Component.VC.ATLMFC `
  --add Microsoft.VisualStudio.Component.VC.CMake.Project `
  --add Microsoft.VisualStudio.Component.Windows11SDK.22621 `
  --includeRecommended --includeOptional --passive --norestart
```
说明：`--includeRecommended --includeOptional` = 安装该工作负载的全部推荐+可选组件（不精简）；显式再加 ATL/ATLMFC/CMake/Windows11 SDK 双保险。等待安装完成（约 10–20 GB 落 C 盘，C 盘空间充足）。

- [ ] **Step 3: 核对关键组件没漏（cl 编译器 + Windows SDK + ATL/MFC）**

Run:
```powershell
$vc = & "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
$ver = (Get-ChildItem "$vc\VC\Tools\MSVC" -Directory | Select-Object -First 1).FullName
"cl.exe:   $(Test-Path "$ver\bin\Hostx64\x64\cl.exe")"
"ATL/MFC:  $(Test-Path "$ver\atlmfc\include\afx.h")"
"SDK:      $((Get-ChildItem 'C:\Program Files (x86)\Windows Kits\10\Include' -Directory -ErrorAction SilentlyContinue | Measure-Object).Count -gt 0)"
```
Expected: 三项均为 `True`。任一 `False` → 回 Step 2 检查对应 `--add` 组件是否成功。

- [ ] **Step 2: 验证 MSVC 编译器存在**

Run:
```powershell
$vc = & "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
$cl = Get-ChildItem "$vc\VC\Tools\MSVC" -Directory | Select-Object -First 1
Test-Path "$($cl.FullName)\bin\Hostx64\x64\cl.exe"
```
Expected: `True`

---

## Task 2: 安装 CMake

**Files:** 无

- [ ] **Step 1: 装 CMake（winget）**

Run:
```powershell
winget install --id Kitware.CMake -e --accept-source-agreements --accept-package-agreements
```
Expected: 安装成功。

- [ ] **Step 2: 新开终端验证（PATH 生效）**

Run（**新开一个 PowerShell**）:
```powershell
cmake --version
```
Expected: `cmake version 3.2x.x`（≥3.16 即可）。若提示找不到，手动把 `C:\Program Files\CMake\bin` 加入 PATH 或重启终端。

---

## Task 3: 下载 Geant4 源码到 E 盘

**Files:** 下载到 `E:\geant4\src`

- [ ] **Step 1: 建目录并下载 v11.3.0 源码 zip**

Run:
```powershell
New-Item -ItemType Directory -Force E:\geant4\src | Out-Null
$zip = "E:\geant4\src\geant4-11.3.0.zip"
Invoke-WebRequest -Uri "https://github.com/Geant4/geant4/archive/refs/tags/v11.3.0.zip" -OutFile $zip
Expand-Archive -Path $zip -DestinationPath E:\geant4\src -Force
Test-Path E:\geant4\src\geant4-11.3.0\CMakeLists.txt
```
Expected: 最后输出 `True`。（如需更新版本，把 `11.3.0` 换成最新 11.x tag。）

---

## Task 4: 编译并安装 Geant4（耗时步骤）

**Files:**
- Create: `E:\code\geant4\setup_env.ps1`

- [ ] **Step 1: CMake 配置（VS 生成器，数据集自动下载到 E 盘）**

Run:
```powershell
cmake -S E:\geant4\src\geant4-11.3.0 -B E:\geant4\build -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_INSTALL_PREFIX=E:\geant4\install `
  -DGEANT4_INSTALL_DATA=ON `
  -DGEANT4_USE_QT=OFF `
  -DGEANT4_BUILD_MULTITHREADED=OFF `
  -DGEANT4_BUILD_TLS_MODEL=global-dynamic `
  -DBUILD_SHARED_LIBS=ON
```
Expected: 末尾 `Configuring done / Generating done`。此步会下载数据集（数 GB），需联网，耗时数分钟。

- [ ] **Step 2: 编译并安装（30 min – 2 h）**

Run:
```powershell
cmake --build E:\geant4\build --config Release --target install -j 8
```
Expected: 末尾 `... -- Installing: E:/geant4/install/...`，无 error。

- [ ] **Step 3: 写 `setup_env.ps1`（自动设置数据集环境变量 + PATH）**

```powershell
# setup_env.ps1 — 运行 betasim 前先 dot-source 本脚本: . .\setup_env.ps1
$G4 = "E:\geant4\install"
# 找到数据目录（share\Geant4-11.3.0\data 或 share\Geant4\data）
$dataRoot = Get-ChildItem "$G4\share" -Directory | Where-Object { Test-Path "$($_.FullName)\data" } | Select-Object -First 1
$data = "$($dataRoot.FullName)\data"
$map = @{
  'G4NDL'                 = 'G4NEUTRONHPDATA'
  'G4EMLOW'               = 'G4LEDATA'
  'PhotonEvaporation'     = 'G4LEVELGAMMADATA'
  'RadioactiveDecay'      = 'G4RADIOACTIVEDATA'
  'G4PARTICLEXS'          = 'G4PARTICLEXSDATA'
  'G4PII'                 = 'G4PIIDATA'
  'RealSurface'           = 'G4REALSURFACEDATA'
  'G4SAIDDATA'            = 'G4SAIDXSDATA'
  'G4ABLA'                = 'G4ABLADATA'
  'G4INCL'                = 'G4INCLDATA'
  'G4ENSDFSTATE'          = 'G4ENSDFSTATEDATA'
}
foreach ($d in Get-ChildItem $data -Directory) {
  foreach ($k in $map.Keys) {
    if ($d.Name -like "$k*") { Set-Item "env:$($map[$k])" $d.FullName }
  }
}
$env:PATH = "$G4\bin;$env:PATH"   # Geant4 DLLs
Write-Host "Geant4 env ready. G4LEDATA=$env:G4LEDATA"
```

- [ ] **Step 4: 用自带例子 B1 验证安装可用**

Run:
```powershell
. E:\code\geant4\setup_env.ps1
cmake -S E:\geant4\src\geant4-11.3.0\examples\basic\B1 -B E:\geant4\b1build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=E:\geant4\install
cmake --build E:\geant4\b1build --config Release
"/run/beamOn 5" | & E:\geant4\b1build\Release\exampleB1.exe
```
Expected: B1 跑完 5 个事件并打印剂量统计，无 DLL/数据缺失报错。**这一步过了就证明工具链彻底就绪。**

---

## Task 5: β 谱采样器（纯 C++，可单测）

**Files:**
- Create: `E:\code\geant4\src\BetaSpectrum.hh`
- Create: `E:\code\geant4\test\test_beta.cc`

- [ ] **Step 1: 写 `src\BetaSpectrum.hh`**

```cpp
#ifndef BETASPECTRUM_HH
#define BETASPECTRUM_HH
#include <vector>
#include <cmath>
// Fermi-theory allowed beta spectrum sampler for Ni-63. Pure C++ (no Geant4).
class BetaSpectrum {
public:
  BetaSpectrum(double endpointKeV = 66.7, int Zdaughter = 29, int nGrid = 4000)
      : fEnd(endpointKeV), fZ(Zdaughter) {
    const double mec2 = 510.998950; // keV
    fE.resize(nGrid);
    fCdf.resize(nGrid);
    const double dE = fEnd / (nGrid - 1);
    double cum = 0.0, prev = 0.0;
    for (int i = 0; i < nGrid; ++i) {
      double E = i * dE;
      double pdf = Shape(E, mec2);
      if (i > 0) cum += 0.5 * (pdf + prev) * dE; // trapezoid integral
      fE[i] = E;
      fCdf[i] = cum;
      prev = pdf;
    }
    const double total = fCdf.back();
    for (auto& c : fCdf) c /= total;
  }
  // u in [0,1) -> kinetic energy in keV (inverse-CDF sampling)
  double Sample(double u) const {
    int lo = 0, hi = (int)fCdf.size() - 1;
    while (lo < hi) { int mid = (lo + hi) / 2; if (fCdf[mid] < u) lo = mid + 1; else hi = mid; }
    if (lo == 0) return fE[0];
    double c0 = fCdf[lo - 1], c1 = fCdf[lo];
    double frac = (c1 > c0) ? (u - c0) / (c1 - c0) : 0.0;
    return fE[lo - 1] + frac * (fE[lo] - fE[lo - 1]);
  }
  double EndpointKeV() const { return fEnd; }
private:
  double Shape(double E, double mec2) const {
    if (E <= 0.0 || E >= fEnd) return 0.0;
    double W = E + mec2;                       // total energy (keV)
    double p = std::sqrt(W * W - mec2 * mec2); // momentum*c (keV)
    if (p <= 0.0) return 0.0;
    double alpha = 1.0 / 137.035999;
    double eta = alpha * fZ * W / p;           // beta- : eta > 0
    double x = 2.0 * M_PI * eta;
    double F = x / (1.0 - std::exp(-x));       // Fermi function (nonrel. Coulomb)
    double q = fEnd - E;
    return F * p * W * q * q;                  // N(E) ∝ F·p·W·(Q-E)^2
  }
  double fEnd;
  int fZ;
  std::vector<double> fE, fCdf;
};
#endif
```

- [ ] **Step 2: 写 `test\test_beta.cc`（失败先行：断言均值范围）**

```cpp
#include "BetaSpectrum.hh"
#include <cstdio>
#include <random>
int main() {
  BetaSpectrum spec(66.7, 29);
  std::mt19937_64 rng(12345);
  std::uniform_real_distribution<double> uni(0.0, 1.0);
  const int N = 2000000;
  double sum = 0.0, emax = 0.0;
  for (int i = 0; i < N; ++i) {
    double e = spec.Sample(uni(rng));
    sum += e;
    if (e > emax) emax = e;
  }
  double mean = sum / N;
  std::printf("mean=%.3f keV  max=%.3f keV  (expect mean~17.4, endpoint 66.7)\n", mean, emax);
  if (mean < 16.5 || mean > 18.5) { std::printf("FAIL: mean out of range\n"); return 1; }
  std::printf("PASS\n");
  return 0;
}
```

- [ ] **Step 3: 临时单独编译并运行测试（不依赖 Geant4）**

Run:
```powershell
. E:\code\geant4\setup_env.ps1   # 仅为引入 MSVC 不需要，但确保终端干净
# 用 cl 直接编译（需在 VS 开发者环境；或用下方 cmake 目标，见 Task 11 一并构建）
cmake -S E:\code\geant4 -B E:\code\geant4\build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=E:\geant4\install 2>$null
```
说明：`test_beta` 目标在 Task 11 的 `CMakeLists.txt` 里定义，构建在 Task 11 进行。**本任务先把文件写好**；均值验证在 Task 11 Step 3 运行 `test_beta.exe` 时完成。

- [ ] **Step 4: 提交**

```powershell
git add src/BetaSpectrum.hh test/test_beta.cc
git commit -m "feat: Ni-63 Fermi beta spectrum sampler + unit test"
```

---

## Task 6: 物理列表（Livermore EM）

**Files:**
- Create: `E:\code\geant4\src\PhysicsList.hh`
- Create: `E:\code\geant4\src\PhysicsList.cc`

- [ ] **Step 1: 写 `src\PhysicsList.hh`**

```cpp
#ifndef PHYSICSLIST_HH
#define PHYSICSLIST_HH
#include "G4VModularPhysicsList.hh"
class PhysicsList : public G4VModularPhysicsList {
public:
  PhysicsList();
  void SetCuts() override;
};
#endif
```

- [ ] **Step 2: 写 `src\PhysicsList.cc`**

```cpp
#include "PhysicsList.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmParameters.hh"
#include "G4ProductionCutsTable.hh"
#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList() {
  RegisterPhysics(new G4EmLivermorePhysics());
  auto* p = G4EmParameters::Instance();
  p->SetMinEnergy(100 * eV);
  p->SetMaxEnergy(1 * MeV);
}

void PhysicsList::SetCuts() {
  // 纳米尺度几何：低产生阈以分辨能量沉积
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100 * eV, 1 * MeV);
  SetCutValue(10 * nm, "e-");
  SetCutValue(10 * nm, "e+");
  SetCutValue(10 * nm, "gamma");
}
```

- [ ] **Step 3: 提交**

```powershell
git add src/PhysicsList.hh src/PhysicsList.cc
git commit -m "feat: Livermore EM physics list with nm-scale cuts"
```

---

## Task 7: 几何与材料（三结构）

**Files:**
- Create: `E:\code\geant4\src\DetectorConstruction.hh`
- Create: `E:\code\geant4\src\DetectorConstruction.cc`

- [ ] **Step 1: 写 `src\DetectorConstruction.hh`**

```cpp
#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
enum class StructureType { TSC, TNTAs, ETNTAs };
class G4Material;
class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  explicit DetectorConstruction(StructureType s);
  ~DetectorConstruction() override = default;
  G4VPhysicalVolume* Construct() override;
  StructureType GetStructure() const { return fStructure; }
  G4double GetHeight() const { return fHeight; }
private:
  void DefineMaterials();
  StructureType fStructure;
  G4double fHeight;
  G4Material* fTiO2 = nullptr;
  G4Material* fElectrolyte = nullptr;
  G4Material* fVacuum = nullptr;
  G4Material* fNi = nullptr;
  G4Material* fTi = nullptr;
};
#endif
```

- [ ] **Step 2: 写 `src\DetectorConstruction.cc`**

```cpp
#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction(StructureType s)
    : fStructure(s), fHeight(12.0 * um) {}

void DetectorConstruction::DefineMaterials() {
  auto* nist = G4NistManager::Instance();
  fVacuum = nist->FindOrBuildMaterial("G4_Galactic");
  fNi = nist->FindOrBuildMaterial("G4_Ni");
  fTi = nist->FindOrBuildMaterial("G4_Ti");

  G4Element* elTi = nist->FindOrBuildElement("Ti");
  G4Element* elO = nist->FindOrBuildElement("O");
  fTiO2 = new G4Material("TiO2_anatase", 3.78 * g / cm3, 2);
  fTiO2->AddElement(elTi, 1);
  fTiO2->AddElement(elO, 2);

  G4Element* elC = nist->FindOrBuildElement("C");
  G4Element* elH = nist->FindOrBuildElement("H");
  G4Element* elN = nist->FindOrBuildElement("N");
  fElectrolyte = new G4Material("Electrolyte_ACN", 0.786 * g / cm3, 3);
  fElectrolyte->AddElement(elC, 2);
  fElectrolyte->AddElement(elH, 3);
  fElectrolyte->AddElement(elN, 1);
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
  DefineMaterials();
  const G4double foot = 1000.0 * nm;
  const G4double H = fHeight;
  const G4double niThick = 200.0 * nm;

  auto* worldS = new G4Box("World", 2.0 * um, 2.0 * um, 12.0 * um);
  auto* worldLV = new G4LogicalVolume(worldS, fVacuum, "World");
  auto* worldPV = new G4PVPlacement(nullptr, {}, worldLV, "World", nullptr, false, 0, true);

  // Ni source slab: bottom at z=0, top at z=niThick
  auto* niS = new G4Box("Ni", foot / 2, foot / 2, niThick / 2);
  auto* niLV = new G4LogicalVolume(niS, fNi, "Ni");
  new G4PVPlacement(nullptr, {0, 0, niThick / 2}, niLV, "Ni", worldLV, false, 0, true);

  // Structure mother: top at z=0, extends to z=-H
  auto* structS = new G4Box("Struct", foot / 2, foot / 2, H / 2);
  G4Material* motherMat = (fStructure == StructureType::TSC) ? fTiO2 : fVacuum;
  auto* structLV = new G4LogicalVolume(structS, motherMat, "Struct");
  new G4PVPlacement(nullptr, {0, 0, -H / 2}, structLV, "Struct", worldLV, false, 0, true);

  if (fStructure != StructureType::TSC) {
    const G4double pitch = 120.0 * nm, rout = 60.0 * nm, rin = 50.0 * nm;
    auto* tubeS = new G4Tubs("Tube", rin, rout, H / 2, 0, 360 * deg);
    auto* tubeLV = new G4LogicalVolume(tubeS, fTiO2, "Tube");
    G4LogicalVolume* elyteLV = nullptr;
    if (fStructure == StructureType::ETNTAs) {
      auto* elyteS = new G4Tubs("Elyte", 0, rin, H / 2, 0, 360 * deg);
      elyteLV = new G4LogicalVolume(elyteS, fElectrolyte, "Elyte");
    }
    int copy = 0;
    for (int i = 0; i < 8; ++i)
      for (int j = 0; j < 8; ++j) {
        G4double x = (i - 3.5) * pitch, y = (j - 3.5) * pitch;
        new G4PVPlacement(nullptr, {x, y, 0}, tubeLV, "Tube", structLV, false, copy, false);
        if (elyteLV)
          new G4PVPlacement(nullptr, {x, y, 0}, elyteLV, "Elyte", structLV, false, copy, false);
        ++copy;
      }
  }

  // Ti substrate below structure (backscatter realism)
  auto* tiS = new G4Box("Sub", foot / 2, foot / 2, 0.5 * um);
  auto* tiLV = new G4LogicalVolume(tiS, fTi, "Sub");
  new G4PVPlacement(nullptr, {0, 0, -H - 0.5 * um}, tiLV, "Sub", worldLV, false, 0, true);

  return worldPV;
}
```

- [ ] **Step 3: 提交**

```powershell
git add src/DetectorConstruction.hh src/DetectorConstruction.cc
git commit -m "feat: TSC/TNTAs/ETNTAs geometry and materials"
```

---

## Task 8: ⁶³Ni β 源（PrimaryGeneratorAction）

**Files:**
- Create: `E:\code\geant4\src\PrimaryGeneratorAction.hh`
- Create: `E:\code\geant4\src\PrimaryGeneratorAction.cc`

- [ ] **Step 1: 写 `src\PrimaryGeneratorAction.hh`**

```cpp
#ifndef PRIMARYGENERATORACTION_HH
#define PRIMARYGENERATORACTION_HH
#include "G4VUserPrimaryGeneratorAction.hh"
#include "BetaSpectrum.hh"
class G4ParticleGun;
class G4Event;
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction() override;
  void GeneratePrimaries(G4Event*) override;
private:
  G4ParticleGun* fGun;
  BetaSpectrum fSpectrum;
};
#endif
```

- [ ] **Step 2: 写 `src\PrimaryGeneratorAction.cc`**

```cpp
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "Randomize.hh"
#include <cmath>

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : fGun(new G4ParticleGun(1)), fSpectrum(66.7, 29) {
  auto* e = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fGun->SetParticleDefinition(e);
}
PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fGun; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* evt) {
  // 位置：Ni 源板体积内均匀采样（1000×1000×200 nm，z∈[0,200nm]）
  G4double x = (G4UniformRand() - 0.5) * 1000.0 * nm;
  G4double y = (G4UniformRand() - 0.5) * 1000.0 * nm;
  G4double z = G4UniformRand() * 200.0 * nm;
  fGun->SetParticlePosition({x, y, z});
  // 方向：向下半球各向同性（cosθ∈[-1,0)）
  G4double cosTheta = -G4UniformRand();
  G4double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
  G4double phi = 2.0 * M_PI * G4UniformRand();
  fGun->SetParticleMomentumDirection(
      {sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta});
  // 能量：Ni-63 β 谱
  G4double E = fSpectrum.Sample(G4UniformRand()) * keV;
  fGun->SetParticleEnergy(E);
  Analysis::Instance().AddEmitted(E / keV);
  fGun->GeneratePrimaryVertex(evt);
}
```

- [ ] **Step 3: 提交**

```powershell
git add src/PrimaryGeneratorAction.hh src/PrimaryGeneratorAction.cc
git commit -m "feat: Ni-63 beta primary source (volume-sampled, downward hemisphere)"
```

---

## Task 9: 数据聚合与 CSV 写出（Analysis 单例）

**Files:**
- Create: `E:\code\geant4\src\Analysis.hh`
- Create: `E:\code\geant4\src\Analysis.cc`

- [ ] **Step 1: 写 `src\Analysis.hh`**

```cpp
#ifndef ANALYSIS_HH
#define ANALYSIS_HH
#include <string>
#include <vector>
#include <tuple>
#include "DetectorConstruction.hh"  // StructureType
class Analysis {
public:
  static Analysis& Instance();
  void Begin(const std::string& prefix, StructureType st);
  void AddEmitted(double Ekev) { fEmittedKE += Ekev; }
  void AddDeposit(double depth_um, double edep_kev, bool electrolyte);
  void AddTrajPoint(long evt, long trk, double x_um, double z_um);
  void AddEdepPoint(double x_um, double z_um, double edep_kev);
  void Write();
  static const int kNBins = 240;
private:
  Analysis() = default;
  std::string fPrefix;
  StructureType fStruct = StructureType::TSC;
  double fBinWidthUm = 12.0 / kNBins;  // 0.05 um
  std::vector<double> fEddTotal{std::vector<double>(kNBins, 0.0)};
  std::vector<double> fEddWall{std::vector<double>(kNBins, 0.0)};
  std::vector<double> fEddElyte{std::vector<double>(kNBins, 0.0)};
  double fEmittedKE = 0.0;
  std::vector<std::tuple<long, long, double, double>> fTraj;     // evt,trk,x,z
  std::vector<std::tuple<double, double, double>> fEdepPts;      // x,z,edep
};
#endif
```

- [ ] **Step 2: 写 `src\Analysis.cc`**

```cpp
#include "Analysis.hh"
#include <fstream>
#include <filesystem>
#include <cmath>

Analysis& Analysis::Instance() {
  static Analysis inst;
  return inst;
}

void Analysis::Begin(const std::string& prefix, StructureType st) {
  fPrefix = prefix;
  fStruct = st;
  std::fill(fEddTotal.begin(), fEddTotal.end(), 0.0);
  std::fill(fEddWall.begin(), fEddWall.end(), 0.0);
  std::fill(fEddElyte.begin(), fEddElyte.end(), 0.0);
  fEmittedKE = 0.0;
  fTraj.clear();
  fEdepPts.clear();
  std::filesystem::create_directories("data");
}

void Analysis::AddDeposit(double depth_um, double edep_kev, bool electrolyte) {
  int b = (int)(depth_um / fBinWidthUm);
  if (b < 0 || b >= kNBins) return;
  fEddTotal[b] += edep_kev;
  if (electrolyte) fEddElyte[b] += edep_kev;
  else fEddWall[b] += edep_kev;
}

void Analysis::AddTrajPoint(long evt, long trk, double x_um, double z_um) {
  fTraj.emplace_back(evt, trk, x_um, z_um);
}
void Analysis::AddEdepPoint(double x_um, double z_um, double edep_kev) {
  fEdepPts.emplace_back(x_um, z_um, edep_kev);
}

void Analysis::Write() {
  const double norm = (fEmittedKE > 0.0) ? fEmittedKE : 1.0;
  // EDD/EDR：edd_norm = (bin能量/总入射能)/箱宽 ；edr = 累计沉积/总入射能
  std::ofstream f("data/" + fPrefix + "_edd_edr.csv");
  bool et = (fStruct == StructureType::ETNTAs);
  f << "depth_um,edd_total,edr_total";
  if (et) f << ",edd_wall,edd_elyte,edr_wall,edr_elyte";
  f << "\n";
  double cum = 0, cumW = 0, cumE = 0;
  for (int i = 0; i < kNBins; ++i) {
    double depth = (i + 0.5) * fBinWidthUm;
    cum += fEddTotal[i]; cumW += fEddWall[i]; cumE += fEddElyte[i];
    f << depth << ","
      << (fEddTotal[i] / norm / fBinWidthUm) << ","
      << (cum / norm);
    if (et)
      f << "," << (fEddWall[i] / norm / fBinWidthUm)
        << "," << (fEddElyte[i] / norm / fBinWidthUm)
        << "," << (cumW / norm) << "," << (cumE / norm);
    f << "\n";
  }
  f.close();

  std::ofstream ft("data/" + fPrefix + "_tracks.csv");
  ft << "event,track,x_um,z_um\n";
  for (auto& [e, t, x, z] : fTraj) ft << e << "," << t << "," << x << "," << z << "\n";
  ft.close();

  std::ofstream fe("data/" + fPrefix + "_edep_points.csv");
  fe << "x_um,z_um,edep_kev\n";
  for (auto& [x, z, ed] : fEdepPts) fe << x << "," << z << "," << ed << "\n";
  fe.close();
}
```

- [ ] **Step 3: 提交**

```powershell
git add src/Analysis.hh src/Analysis.cc
git commit -m "feat: Analysis singleton with EDD/EDR histograms and CSV writers"
```

---

## Task 10: 事件与步进打分（EventAction + SteppingAction）

**Files:**
- Create: `E:\code\geant4\src\EventAction.hh`
- Create: `E:\code\geant4\src\EventAction.cc`
- Create: `E:\code\geant4\src\SteppingAction.hh`
- Create: `E:\code\geant4\src\SteppingAction.cc`

- [ ] **Step 1: 写 `src\EventAction.hh`**

```cpp
#ifndef EVENTACTION_HH
#define EVENTACTION_HH
#include "G4UserEventAction.hh"
#include "globals.hh"
class EventAction : public G4UserEventAction {
public:
  explicit EventAction(long nTraj) : fNTraj(nTraj) {}
  void BeginOfEventAction(const G4Event* e) override;
  G4bool Record() const { return fRecord; }
  long EventID() const { return fEventID; }
private:
  long fNTraj;
  long fEventID = 0;
  G4bool fRecord = false;
};
#endif
```

- [ ] **Step 2: 写 `src\EventAction.cc`**

```cpp
#include "EventAction.hh"
#include "G4Event.hh"
void EventAction::BeginOfEventAction(const G4Event* e) {
  fEventID = e->GetEventID();
  fRecord = (fEventID < fNTraj);  // 仅前 nTraj 个事件记录轨迹
}
```

- [ ] **Step 3: 写 `src\SteppingAction.hh`**

```cpp
#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH
#include "G4UserSteppingAction.hh"
class EventAction;
class SteppingAction : public G4UserSteppingAction {
public:
  explicit SteppingAction(EventAction* ev) : fEvent(ev) {}
  void UserSteppingAction(const G4Step*) override;
private:
  EventAction* fEvent;
};
#endif
```

- [ ] **Step 4: 写 `src\SteppingAction.cc`**

```cpp
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"
#include "G4Step.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"

void SteppingAction::UserSteppingAction(const G4Step* step) {
  G4double edep = step->GetTotalEnergyDeposit();
  auto* pre = step->GetPreStepPoint();
  G4Material* m = pre->GetMaterial();
  bool isTiO2 = (m && m->GetName() == "TiO2_anatase");
  bool isElyte = (m && m->GetName() == "Electrolyte_ACN");

  G4ThreeVector p0 = pre->GetPosition();
  G4ThreeVector p1 = step->GetPostStepPoint()->GetPosition();
  G4ThreeVector mid = 0.5 * (p0 + p1);
  double depth_um = -mid.z() / um;  // 结构在 z<0

  if ((isTiO2 || isElyte) && edep > 0 && depth_um >= 0 && depth_um < 12.0) {
    Analysis::Instance().AddDeposit(depth_um, edep / keV, isElyte);
  }

  if (fEvent->Record()) {
    long trk = step->GetTrack()->GetTrackID();
    Analysis::Instance().AddTrajPoint(fEvent->EventID(), trk, p0.x() / um, p0.z() / um);
    // 记录径迹末点，保证折线连到终点
    Analysis::Instance().AddTrajPoint(fEvent->EventID(), trk, p1.x() / um, p1.z() / um);
    if (edep > 0)
      Analysis::Instance().AddEdepPoint(mid.x() / um, mid.z() / um, edep / keV);
  }
}
```

- [ ] **Step 5: 提交**

```powershell
git add src/EventAction.hh src/EventAction.cc src/SteppingAction.hh src/SteppingAction.cc
git commit -m "feat: stepping-level EDD/EDR scoring + trajectory capture"
```

---

## Task 11: 装配、入口、构建系统（首次编译运行）

**Files:**
- Create: `E:\code\geant4\src\RunAction.hh`
- Create: `E:\code\geant4\src\RunAction.cc`
- Create: `E:\code\geant4\src\ActionInitialization.hh`
- Create: `E:\code\geant4\src\ActionInitialization.cc`
- Create: `E:\code\geant4\src\main.cc`
- Create: `E:\code\geant4\CMakeLists.txt`

- [ ] **Step 1: 写 `src\RunAction.hh` / `src\RunAction.cc`**

```cpp
// RunAction.hh
#ifndef RUNACTION_HH
#define RUNACTION_HH
#include "G4UserRunAction.hh"
class RunAction : public G4UserRunAction {
public:
  void BeginOfRunAction(const G4Run*) override;
  void EndOfRunAction(const G4Run*) override;
};
#endif
```
```cpp
// RunAction.cc
#include "RunAction.hh"
#include "G4Run.hh"
#include "G4ios.hh"
void RunAction::BeginOfRunAction(const G4Run*) { G4cout << "[run] start" << G4endl; }
void RunAction::EndOfRunAction(const G4Run* run) {
  G4cout << "[run] done, events=" << run->GetNumberOfEvent() << G4endl;
}
```

- [ ] **Step 2: 写 `src\ActionInitialization.hh` / `.cc`**

```cpp
// ActionInitialization.hh
#ifndef ACTIONINITIALIZATION_HH
#define ACTIONINITIALIZATION_HH
#include "G4VUserActionInitialization.hh"
class ActionInitialization : public G4VUserActionInitialization {
public:
  explicit ActionInitialization(long nTraj) : fNTraj(nTraj) {}
  void Build() const override;
private:
  long fNTraj;
};
#endif
```
```cpp
// ActionInitialization.cc
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "RunAction.hh"
void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction());
  auto* evt = new EventAction(fNTraj);
  SetUserAction(evt);
  SetUserAction(new RunAction());
  SetUserAction(new SteppingAction(evt));
}
```

- [ ] **Step 3: 写 `src\main.cc`**

```cpp
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
#include "Analysis.hh"
#include "G4RunManagerFactory.hh"
#include "Randomize.hh"
#include <string>
#include <cstdlib>

int main(int argc, char** argv) {
  // 用法: betasim <TSC|TNTAs|ETNTAs> <nEvents> <nTraj> <prefix>
  std::string structure = (argc > 1) ? argv[1] : "TSC";
  long nEvents = (argc > 2) ? std::atol(argv[2]) : 100000;
  long nTraj = (argc > 3) ? std::atol(argv[3]) : 200;
  std::string prefix = (argc > 4) ? argv[4] : structure;

  StructureType st = StructureType::TSC;
  if (structure == "TNTAs") st = StructureType::TNTAs;
  else if (structure == "ETNTAs") st = StructureType::ETNTAs;

  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(20260606);

  auto* rm = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);
  rm->SetUserInitialization(new DetectorConstruction(st));
  rm->SetUserInitialization(new PhysicsList());
  rm->SetUserInitialization(new ActionInitialization(nTraj));

  Analysis::Instance().Begin(prefix, st);
  rm->Initialize();
  rm->BeamOn(nEvents);
  Analysis::Instance().Write();

  delete rm;
  return 0;
}
```

- [ ] **Step 4: 写 `CMakeLists.txt`**

```cmake
cmake_minimum_required(VERSION 3.16)
project(BetavoltaicSim CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cc)
add_executable(betasim ${SRC})
target_include_directories(betasim PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(betasim ${Geant4_LIBRARIES})

# 纯 C++ 单测（不链 Geant4）
add_executable(test_beta ${PROJECT_SOURCE_DIR}/test/test_beta.cc)
target_include_directories(test_beta PRIVATE ${PROJECT_SOURCE_DIR}/src)
```

- [ ] **Step 5: 配置并编译**

Run:
```powershell
. E:\code\geant4\setup_env.ps1
cmake -S E:\code\geant4 -B E:\code\geant4\build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=E:\geant4\install
cmake --build E:\code\geant4\build --config Release
```
Expected: 生成 `build\Release\betasim.exe` 与 `build\Release\test_beta.exe`，无 error。

- [ ] **Step 6: 运行 β 谱单测（验证均值 ≈17.4 keV）**

Run:
```powershell
E:\code\geant4\build\Release\test_beta.exe
```
Expected: 打印 `mean=17.xxx keV ... PASS`。若 FAIL → 检查 `BetaSpectrum::Shape`。

- [ ] **Step 7: 几何 overlap 自检 + 跑通 1 个小规模 run**

Run:
```powershell
cd E:\code\geant4
.\build\Release\betasim.exe TSC 2000 50 TSC
```
Expected: 无 overlap 警告（构造时 checkOverlaps=true 的大体积无重叠）；生成 `data\TSC_edd_edr.csv` / `TSC_tracks.csv` / `TSC_edep_points.csv`。

- [ ] **Step 8: 提交**

```powershell
git add src/RunAction.* src/ActionInitialization.* src/main.cc CMakeLists.txt setup_env.ps1
git commit -m "feat: wire actions, entry point, CMake build; first working run"
```

---

## Task 12: 能量守恒自检（物理正确性）

**Files:** 无新文件（验证 + 临时脚本）

- [ ] **Step 1: 跑 TSC 中等规模并检查 EDR 饱和值**

Run:
```powershell
cd E:\code\geant4
.\build\Release\betasim.exe TSC 50000 0 TSC
python -c "import csv; r=list(csv.DictReader(open('data/TSC_edd_edr.csv'))); print('EDR_max=%.3f (TSC 期望~0.85-0.92)'%float(r[-1]['edr_total']))"
```
Expected: `EDR_max` 落在约 0.80–0.95（其余为背散射/Ni 自吸收/穿透损失）。若远低于（如 ~0.5）说明方向采样把太多粒子射飞 → 复查 `PrimaryGeneratorAction` 的向下半球逻辑。

- [ ] **Step 2: 检查 EDD 峰位置（TSC 期望峰在 ~0.9 μm）**

Run:
```powershell
python -c "import csv; r=list(csv.DictReader(open('data/TSC_edd_edr.csv'))); p=max(r,key=lambda x:float(x['edd_total'])); print('EDD 峰深度=%.2f um (期望~0.9)'%float(p['depth_um']))"
```
Expected: 峰深度约 0.5–1.5 μm。

---

## Task 13: 一键运行三结构（生成完整数据）

**Files:**
- Create: `E:\code\geant4\scripts\run_all.ps1`

- [ ] **Step 1: 写 `scripts\run_all.ps1`**

```powershell
# scripts\run_all.ps1 — 一键跑三结构并出图
param(
  [int]$NEvents = 200000,   # EDD/EDR 统计用事件数
  [int]$NTraj = 200         # 轨迹图记录的事件数
)
$root = Split-Path $PSScriptRoot -Parent
. "$root\setup_env.ps1"
$exe = "$root\build\Release\betasim.exe"
foreach ($s in @("TSC", "TNTAs", "ETNTAs")) {
  Write-Host "=== Running $s ($NEvents events) ==="
  & $exe $s $NEvents $NTraj $s
}
Write-Host "=== Plotting ==="
python "$root\analysis\plot.py"
Write-Host "Done. 图在 $root\figures"
```

- [ ] **Step 2: 先用小规模验证脚本贯通（出数据，不看质量）**

Run:
```powershell
E:\code\geant4\scripts\run_all.ps1 -NEvents 5000 -NTraj 50
```
Expected: 依次跑完 TSC/TNTAs/ETNTAs，`data\` 下出现 9 个 CSV（每结构 3 个）。`plot.py` 此时尚未写好会报错——下一任务补上后再整体跑。本步只验证三结构都能跑出 CSV。

- [ ] **Step 3: 提交**

```powershell
git add scripts/run_all.ps1
git commit -m "feat: run_all.ps1 to run all three structures"
```

---

## Task 14: 绘图脚本 — EDD 与 EDR

**Files:**
- Create: `E:\code\geant4\analysis\requirements.txt`
- Create: `E:\code\geant4\analysis\plot.py`

- [ ] **Step 1: 写 `analysis\requirements.txt` 并安装**

```text
numpy
matplotlib
```
Run:
```powershell
pip install -r E:\code\geant4\analysis\requirements.txt
```

- [ ] **Step 2: 写 `analysis\plot.py`（EDD + EDR + 验证打印；轨迹图在 Task 15 追加）**

```python
import csv, os
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA = os.path.join(ROOT, "data")
FIG = os.path.join(ROOT, "figures")
os.makedirs(FIG, exist_ok=True)

STRUCTS = ["TSC", "TNTAs", "ETNTAs"]
COLORS = {"TSC": "#1f77b4", "TNTAs": "#d62728", "ETNTAs": "#2ca02c"}


def load_edd(s):
    path = os.path.join(DATA, f"{s}_edd_edr.csv")
    rows = list(csv.DictReader(open(path)))
    d = np.array([float(r["depth_um"]) for r in rows])
    edd = np.array([float(r["edd_total"]) for r in rows])
    edr = np.array([float(r["edr_total"]) for r in rows])
    return d, edd, edr


def plot_edd():
    plt.figure(figsize=(6, 4.5))
    for s in STRUCTS:
        d, edd, _ = load_edd(s)
        plt.plot(d, edd, label=s, color=COLORS[s], lw=1.8)
    plt.xlabel("Depth (μm)")
    plt.ylabel("EDD (norm. / μm)")
    plt.title("Energy Deposition Density vs Depth")
    plt.legend(); plt.grid(alpha=0.3); plt.xlim(0, 12)
    plt.tight_layout()
    plt.savefig(os.path.join(FIG, "EDD.png"), dpi=200)
    plt.close()


def plot_edr():
    plt.figure(figsize=(6, 4.5))
    info = {}
    for s in STRUCTS:
        d, _, edr = load_edd(s)
        plt.plot(d, edr, label=s, color=COLORS[s], lw=1.8)
        emax = edr[-1]
        # 最优高度：EDR 达到 95% 饱和值的深度
        idx = np.argmax(edr >= 0.95 * emax)
        info[s] = (emax, d[idx])
    plt.axvline(9.0, color="gray", ls="--", lw=1, label="9 μm (paper opt.)")
    plt.xlabel("Depth (μm)")
    plt.ylabel("EDR (cumulative fraction)")
    plt.title("Energy Deposition Ratio vs Depth")
    plt.legend(); plt.grid(alpha=0.3); plt.xlim(0, 12)
    plt.tight_layout()
    plt.savefig(os.path.join(FIG, "EDR.png"), dpi=200)
    plt.close()
    print("=== 验证（对照论文）===")
    for s in STRUCTS:
        emax, opt = info[s]
        print(f"{s}: EDR_max={emax:.3f}, 95%饱和深度≈{opt:.1f} μm")


if __name__ == "__main__":
    plot_edd()
    plot_edr()
    print("EDD.png / EDR.png saved to figures/")
```

- [ ] **Step 3: 运行出 EDD/EDR 图**

Run:
```powershell
python E:\code\geant4\analysis\plot.py
```
Expected: `figures\EDD.png`、`figures\EDR.png` 生成；终端打印各结构 EDR_max 与 95% 饱和深度。

- [ ] **Step 4: 提交**

```powershell
git add analysis/requirements.txt analysis/plot.py
git commit -m "feat: matplotlib EDD/EDR plots + paper-comparison printout"
```

---

## Task 15: 绘图脚本 — 轨迹图

**Files:**
- Modify: `E:\code\geant4\analysis\plot.py`（追加轨迹绘制）

- [ ] **Step 1: 在 `plot.py` 追加轨迹绘制函数**

在 `if __name__` 之前插入：
```python
def plot_tracks():
    fig, axes = plt.subplots(1, 3, figsize=(13, 4.5), sharey=True)
    for ax, s in zip(axes, STRUCTS):
        # 轨迹折线（按 event,track 分组）
        tpath = os.path.join(DATA, f"{s}_tracks.csv")
        rows = list(csv.DictReader(open(tpath)))
        groups = {}
        for r in rows:
            key = (r["event"], r["track"])
            groups.setdefault(key, []).append((float(r["x_um"]), float(r["z_um"])))
        for pts in groups.values():
            xs = [p[0] for p in pts]
            zs = [p[1] for p in pts]
            ax.plot(xs, zs, color="red", lw=0.3, alpha=0.6)
        # 能量沉积点（黄点）
        epath = os.path.join(DATA, f"{s}_edep_points.csv")
        er = list(csv.DictReader(open(epath)))
        if er:
            ex = [float(r["x_um"]) for r in er]
            ez = [float(r["z_um"]) for r in er]
            ax.scatter(ex, ez, s=2, color="gold", alpha=0.5, edgecolors="none")
        # 结构轮廓
        ax.axhspan(0, 0.2, xmin=0, xmax=1, color="gray", alpha=0.15)  # Ni 源示意
        ax.axhline(0, color="k", lw=0.5)
        ax.axhline(-9.0, color="green", ls="--", lw=0.8)  # 9μm 参考
        ax.set_title(s)
        ax.set_xlabel("x (μm)")
        ax.set_xlim(-0.6, 0.6)
        ax.set_ylim(-12, 0.5)
    axes[0].set_ylabel("z / depth (μm)")
    fig.suptitle("Beta Trajectories (red) & Energy-Deposition Events (yellow)")
    plt.tight_layout()
    plt.savefig(os.path.join(FIG, "trajectories.png"), dpi=200)
    plt.close()
```
并把 `__main__` 改为：
```python
if __name__ == "__main__":
    plot_edd()
    plot_edr()
    plot_tracks()
    print("EDD.png / EDR.png / trajectories.png saved to figures/")
```

- [ ] **Step 2: 运行验证轨迹图**

Run:
```powershell
python E:\code\geant4\analysis\plot.py
```
Expected: 新增 `figures\trajectories.png`，可见红色径迹 + 黄色沉积点，TSC 集中在浅层、TNTAs/ETNTAs 更深更分散。

- [ ] **Step 3: 提交**

```powershell
git add analysis/plot.py
git commit -m "feat: trajectory + energy-deposition-event plots (Fig.2 style)"
```

---

## Task 16: 完整运行、对照论文验证、收尾

**Files:**
- Modify: `E:\code\geant4\README.md`

- [ ] **Step 1: 完整规模跑全流程**

Run:
```powershell
E:\code\geant4\scripts\run_all.ps1 -NEvents 200000 -NTraj 200
```
Expected: 三结构跑完（可能数分钟–数十分钟），`figures\` 下生成 EDD.png / EDR.png / trajectories.png。

- [ ] **Step 2: 对照论文核对（验收）**

人工核对终端打印与图：
- [ ] β 谱均值 ≈ 17.4 keV（test_beta 已验）。
- [ ] TSC：EDD 峰浅（~0.9 μm），EDR 在 ~2 μm 已接近饱和（论文 ~90%/2μm）。
- [ ] TNTAs：EDD 谱更宽，深处仍有沉积（论文 ~76%/10μm）。
- [ ] E-TNTAs：管壁部分峰约 2 μm；电解液部分吸收占比小（论文管壁~60%/9μm、电解液~16%/5μm）。
- [ ] EDR 曲线的 95% 饱和深度与 9 μm 参考线量级吻合（定性支持"最优高度≈9μm"）。

> 若个别数值偏差较大：在 README "已知偏差" 记录原因（直管近似、电解液简化、源自吸收模型差异），符合设计文档"以趋势复现为验收口径"。

- [ ] **Step 3: 完善 `README.md`（安装→构建→运行→出图全流程）**

```markdown
# Betavoltaic Fig.2 Reproduction (Geant4)

复现 Composites Part B 239 (2022) 109952 Fig.2：⁶³Ni β 在 TSC/TNTAs/E-TNTAs 的
能量沉积（EDD/EDR）与轨迹。原生 C++ Geant4 + Livermore 物理；Python 出图。

## 依赖
- Geant4 11.3.x（装于 `E:\geant4\install`）
- Visual Studio 2022（C++ 工作负载）、CMake
- Python 3.13 + numpy + matplotlib

## 构建
```powershell
. .\setup_env.ps1
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=E:\geant4\install
cmake --build build --config Release
```

## 运行 & 出图
```powershell
.\scripts\run_all.ps1 -NEvents 200000 -NTraj 200
```
图输出到 `figures\`：`EDD.png` / `EDR.png` / `trajectories.png`。

## 单测
```powershell
.\build\Release\test_beta.exe   # β 谱均值 ≈ 17.4 keV
```

## 已知偏差
- 几何用直管近似（论文为锥形管腔）。
- 电解液近似为乙腈（论文为乙腈基多碘电解液）。
- 数值以趋势/量级复现为准。
```

- [ ] **Step 4: 最终提交**

```powershell
git add README.md
git commit -m "docs: finalize README; full Fig.2 reproduction complete"
```

---

## 自检（计划对照设计文档）

- **范围覆盖**：三结构几何（Task 7）✓；Livermore 物理（Task 6）✓；⁶³Ni β 谱源（Task 5/8）✓；EDD/EDR 打分（Task 9/10）✓；轨迹（Task 10/15）✓；CSV→matplotlib（Task 14/15）✓；安装工具链（Task 1-4）✓；验收对照（Task 12/16）✓。
- **类型一致**：`StructureType{TSC,TNTAs,ETNTAs}` 全程一致；`Analysis` 方法签名（AddEmitted/AddDeposit/AddTrajPoint(evt,trk,x,z)/AddEdepPoint）在 SteppingAction/PrimaryGenerator 调用处一致；CSV 列名与 plot.py 读取键一致（depth_um/edd_total/edr_total/event/track/x_um/z_um/edep_kev）。
- **无占位符**：各步含完整代码与确切命令、期望输出。
```
