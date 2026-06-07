# Hermes 上手指南 — β 能量沉积仿真(Windows)

欢迎!这份文件教你(Hermes)从零把这套 **β 伏特能量沉积 Geant4 仿真**装好、跑通、学会。
你不需要懂 C++——所有事让 Claude Code 配合项目里的 `betavoltaic-sim` 技能帮你做。

> 复现论文:N. Wang et al., *Composites Part B* 239 (2022) 109952(TiO₂ 纳米管 β 伏特电池 Fig.2)。
> 全貌见 `docs/report.html`(浏览器打开)、`README.md`、`docs/superpowers/specs/*.md`。

---

## 0. 前置(你机器需要的)

| 项 | 要求 |
|---|---|
| 操作系统 | Windows 10/11 |
| Visual Studio 2022 | 已装(Community 即可);C++ 组件可由脚本静默补装 |
| 磁盘 | C 盘留 **≥20GB**(VS C++ 组件);另准备一个盘放 Geant4(源码+数据~3GB+编译产物,建议 ≥15GB) |
| Python | 3.10+(出图用) |
| Git | 用于 clone |

---

## 1. 克隆仓库

```powershell
git clone https://github.com/<your-org>/betavoltaic-sim.git
cd betavoltaic-sim
```

---

## 2. 在该目录打开 Claude Code,粘贴这段"教学提示词"

> 这是核心。把下面整段发给 Claude Code,它会用 `betavoltaic-sim` 技能带你走完全程、带检查点、并用大白话讲解。

```
我刚 clone 了 betavoltaic-sim 仓库(Windows 机器)。请用项目里的
betavoltaic-sim 技能,带我从零搭好并学会这套 β 能量沉积仿真:

1) 先读 README.md 和 docs/report.html、docs/superpowers/specs/*.md 了解全貌;
2) 按"批次0 环境"带我装好 VS C++ 组件、编译 Geant4 11.3.2(静态库,装到我指定的盘)、
   装 Python 依赖——每一步停下来等我确认,装 VS 要提权我会点 UAC;
3) 编译 betasim,跑 test_beta 自检 β 谱(应 ≈17.5 keV);
4) 跑黄金测试三结构 + 出图,用大白话给我讲每张图(EDD/EDR/深度分布/轨迹)
   的物理含义,以及"最优纳米管高度"是怎么读出来的;
5) 教我怎么改 configs/*.yaml 跑我自己的材料(我想先试 ZnO 纳米线 + Ni-63);
6) 诚实告诉我哪些数值和论文吻合、哪些有偏差及原因(别替我粉饰)。

我的环境:Windows 11,C 盘留 ≥20GB,Geant4 装到 <填你的盘,如 E:\geant4>。
遇到需要我操作的(如 UAC、装组件)请明确告诉我点什么。
```

---

## 3. 关键路径需要你确认/修改

脚本里有几处硬编码路径,clone 到你机器后按需改:

| 文件 | 变量 | 改成你的 |
|---|---|---|
| `scripts/install_vs.ps1` | `$vsPath` | 你的 VS2022 安装路径(默认 `F:\Program Files\Microsoft Visual Studio\2022\Community`) |
| `scripts/build_geant4.ps1` | `$Src/$Build/$Install/$VsPath` | Geant4 源码/编译/安装盘 + VS 路径 |
| `setup_env.ps1` | `$G4Install` | Geant4 安装目录 |

> 让 Claude 帮你探测(`vswhere`、磁盘空间)后自动改这些。

---

## 4. 你会学到什么

- **EDD(能量沉积密度)/ EDR(能量沉积比)** 是什么、怎么从曲线读出**最优结构高度**;
- 三种结构(实心 TSC / 空心纳米管 TNTAs / 填电解液 E-TNTAs)的物理差异;
- **只改 `config.yaml`** 就能换同位素(Ni-63/H-3/C-14/Pm-147)、换材料、换几何、换入射模型,不碰 C++;
- 诚实的科研态度:哪些和论文吻合、哪些有偏差、为什么(见 `docs/superpowers/specs/2026-06-06-validation-report.md`)。

---

## 5. Windows 已知坑(脚本已处理,供你了解)

1. VS `setup.exe modify` **不接受 `--wait`**(传了→退出码 87)→ 脚本用 `ProcessStartInfo + WaitForExit`。
2. MSVC 读 UTF-8 中文注释须 **`/utf-8`** → CMake 已加 `if(MSVC) add_compile_options(/utf-8)`。
3. PowerShell 5.1 读无 BOM 的 .ps1 按 GBK 解析 → 被 `-File` 调用的脚本用纯 ASCII 注释。
4. Geant4 在 MSVC 上须 **静态库**(`BUILD_SHARED_LIBS=OFF`)否则 `G4processes.dll` 触发 LNK1189。
5. 中文 PDF 文件名传参会变 `????` → 用 `glob` 取路径;读 PDF 图用 **PyMuPDF**(`pip install pymupdf`),本机无 `pdftoppm`。

有问题就把报错贴给 Claude,它能照着 `docs/` 里的踩坑记录帮你排。
