---
name: betavoltaic-sim
description: Use when a materials researcher wants to run a β-voltaic / beta energy-deposition Geant4 simulation — energy deposition density/ratio (EDD/EDR), particle trajectories, and the optimal nanostructure height — for a radioisotope β source (Ni-63 / H-3 / C-14 / Pm-147) in a nanostructure (solid cuboid / nanotube array / nanowire array, optionally electrolyte-filled). Drives the whole workflow (environment setup → goal interview → config → run → plot → interpret) on a validated, parameterized engine so the user never writes C++.
---

# Betavoltaic Energy-Deposition Simulation (β 能量沉积仿真)

帮材料研究者**不写一行 C++** 就完成可靠的 β 能量沉积 Geant4 仿真。可靠性来自一个**编译一次、对照论文验证过的参数化引擎**——研究者只改一份 `config.yaml`,你负责把"大白话目标"翻译成配置并跑通全流程。

**论文基准**:Wang et al., *Betavoltaic-powered electrochemical cells using TiO₂ nanotube arrays incorporated with carbon nanotubes*, Composites Part B 239 (2022) 109952(Fig.2 能量沉积)。

## 何时用本技能
- 用户想知道某同位素 β 源打在某纳米结构里的**能量沉积深度分布 / 最优结构高度 / 轨迹**。
- 用户想换材料、换同位素、换几何尺寸重跑这类仿真。
- 用户要搭/装这套仿真环境。

## 引擎与脚本(项目根 `E:\code\geant4`)
| 部件 | 路径 | 作用 |
|------|------|------|
| 参数化引擎 | `build\betasim.exe` | 读宏 → 跑仿真 → 写 CSV(几何/同位素/材料全参数化) |
| β 谱单测 | `build\test_beta.exe` | 自检:Ni-63 平均≈17.4keV、端点≈66.7keV |
| 配置转换 | `analysis\yaml_to_mac.py` | `config.yaml` → `run.mac` |
| 出图 | `analysis\plot.py` | EDD/EDR/轨迹图 + 自检打印 |
| 环境变量 | `setup_env.ps1` | 运行前载入 Geant4 数据集 + PATH |
| 一键黄金测试 | `scripts\run_all.ps1` | 编译+三结构+出图 |

## 关键纪律(务必遵守)
1. **绝不修改 `src/` 里的 C++**。所有变化只通过 `config.yaml`。若用户需求超出参数范围(如全新几何),明确告知这是引擎扩展(方案 B 范畴),需另行评估,不要临时乱改代码。
2. **每次给结果前先跑自检**:运行 `test_beta.exe` 看 β 谱平均能量;运行后看 `plot.py` 打印的 EDR 百分比是否在合理量级。
3. **每个阶段在检查点停下**,把结果给用户确认再继续。
4. 数值口径:**趋势/量级**为准(几何用直管近似、电解液用乙腈近似),不追求绝对值与论文逐位吻合。

---

## 阶段 0 · 环境(带检查点)
目标:`betasim.exe` 能编译运行、Python 能出图。

1. **探查**:VS C++ 工具链(`cl.exe`)、Geant4 安装(`E:\geant4\install`)、Python+numpy/matplotlib/pyyaml。
2. **缺 VS C++ 组件**:静默安装(需管理员,用 `Start-Process -Verb RunAs` 触发 UAC,用户点一次"是")。组件:`VC.Tools.x86.x64` + `Windows11SDK.22621` + `VC.CMake.Project`。详见 GOAL §3.2。若提权传参出 87 错,改用"写 .bat 再提权跑 bat"的方式。
3. **缺 Geant4**:源码编译装到 E 盘(`-DCMAKE_INSTALL_PREFIX=E:\geant4\install -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_QT=OFF`),耗时 30min–2h,放后台。
4. **缺 Python 库**:`pip install -r analysis\requirements.txt`。
5. **构建引擎**:`. .\setup_env.ps1` 后 `cmake -S . -B build -G Ninja` + `cmake --build build`。
6. **⏸ 检查点**:跑 `test_beta.exe`,确认 Ni-63 平均≈17.4keV。通过则报告"环境就绪"。

## 阶段 1 · 访谈式问目标(一次问一个,优先选择题)
把研究者的意图问清楚,逐项落到配置字段。建议问:
- **同位素源**?(Ni-63 / H-3 / C-14 / Pm-147)
- **结构类型**?(实心 / 纳米管阵列 / 纳米线阵列;管阵列是否填电解液)
- **材料**?(TiO2_anatase / ZnO / Si / SiC / 其他 NIST 材料)
- **几何尺寸**?(高度、管内外半径、间距、阵列大小)——给论文默认值兜底。
- **想得到什么结论**?(最优高度 / 三结构对比 / 仅某一结构的 EDD-EDR)
- **统计量**?(n_events,默认 5e4;追求平滑曲线可加大)

把"想得到什么"翻成跑几次:对比多结构=多份配置;找最优高度=结构高度设得比预期高(如 12μm)以便从 EDR 曲线定位。

## 阶段 2 · 生成配置
按访谈结果写 `configs\<名字>.yaml`(字段见 `configs\example_*.yaml`)。给用户看一遍配置,确认。**⏸ 检查点**。

## 阶段 3 · 运行
```powershell
. .\setup_env.ps1
python analysis\yaml_to_mac.py configs\<名字>.yaml -o run.mac
.\build\betasim.exe run.mac
```
对比多结构则对每份配置重复。**⏸ 检查点**:看 betasim 末尾打印的 self-check(EDD 峰深度、EDR@2/9/10μm)。

## 阶段 4 · 出图
```powershell
python analysis\plot.py --prefixes data\<前缀>=<标签> [...]
```
生成 `figures\` 下轨迹图 + EDD + EDR。**⏸ 检查点**:把图给用户。

## 阶段 5 · 解读(用大白话)
读 `plot.py` 打印的 EDR 百分比,向研究者解释:
- **最优结构高度**:EDR 曲线接近饱和的拐点深度(β 能量基本被吸收完;再高=浪费材料)。
- **结构对比**:实心吸收浅而集中;空心管阵列谱更宽、部分穿透;填电解液后等效密度居中。
- **与论文对照**:TSC ~90%@2μm、TNTAs ~76%@10μm、E-TNTAs 管壁 ~60%@9μm、电解液 ~16%、最优 ≈9μm。
- 给出**对该研究者材料的建议**(该把结构做多高最划算)。

---

## 验证基准(给你判断"结果是否可信")
- β 谱:Ni-63 平均 17.4±1.5 keV、端点 ≈66.98 keV。
- EDR 量级:TSC@2μm 接近 ~90%(背散射损失 ~10%);全空心管阵列穿透更多、饱和值更低。
- 若数值严重偏离(如 EDR>100%、平均能量翻倍),先查:cuts 是否过大、几何是否退化、源是否在结构外。

## 已知近似与边界(YAGNI)
直管近似(非锥形管腔)、乙腈近似真实多碘电解液、无 Qt 三维可视化、单线程、不做载流子收集等器件级建模。需要这些 = 引擎扩展,另行评估。
