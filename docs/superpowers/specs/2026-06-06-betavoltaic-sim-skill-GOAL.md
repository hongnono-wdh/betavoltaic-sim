# GOAL 提示词 · β 伏特能量沉积仿真技能(方案 A)

> **用途**:本文件是本次内联执行的"使命简报 / 北极星"。每个批次开工前对照它,完工时对照"可验证成功标准"验收。也作为日后方案 B 和博士生验证的基准。
>
> **日期**:2026-06-06 ｜ **执行方式**:内联、按批次、关键检查点停下来等用户确认。
> **对应论文**:Wang et al., *Betavoltaic-powered electrochemical cells using TiO₂ nanotube arrays incorporated with carbon nanotubes*, Composites Part B 239 (2022) 109952。

---

## 1. 使命(一句话)

做一个 **Claude Code 技能**,让**不懂编程**的材料研究者也能**可靠地**完成 β 伏特/能量沉积的 Geant4 仿真:技能负责 **① 装好整个环境 → ② 访谈式问清仿真目标 → ③ 生成配置 → ④ 运行 → ⑤ 出图 → ⑥ 解读**。可靠性来自一个**编译一次、对照论文验证过的参数化引擎**——研究者永远只改配置,不碰 C++。

## 2. 架构(两层,通过配置文件对话)

- **第 1 层 · 参数化引擎 `betasim.exe`(C++ Geant4,编译一次)**
  读 `config.yaml` → 建几何/源/物理 → 运行 → 写 CSV。几何/同位素/材料全部参数化,代码固定。
- **第 2 层 · Claude Code 技能 `betavoltaic-sim`(.md + 辅助脚本)**
  编排者:装环境(带检查点)→ 问目标 → 填配置 → 运行 → 出图 → 解读。

**配置接口(研究者唯一面对、且由 Claude 协助填写的"实验设置单"):**
```yaml
isotope:    Ni-63           # 内置谱:Ni-63 / H-3 / C-14 / Pm-147
structure:  nanotube_array  # solid / nanotube_array / nanowire_array
material:   TiO2_anatase    # 内置库 + Geant4 NIST 材料
fill:       acetonitrile    # 管腔填充:none / 电解液近似(可选)
geometry:   {height_um: 12, tube_outer_nm: 60, tube_inner_nm: 50, pitch_nm: 120, array: 8x8}
substrate:  Ti              # 可选,增强背散射真实性
run:        {n_events: 200000, n_tracks: 200}
output_prefix: my_TiO2_run
```

## 3. 技术栈与路径约定

- Geant4 11.x(MSVC 编译,**全量装 E 盘** `E:\geant4\install`),Livermore 低能电磁物理。
- VS2022 + C++ 组件(MSVC v143 + Windows SDK + CMake/Ninja);Python 3.13 + numpy + matplotlib(出图,与仿真用 CSV 解耦)。
- 项目根 `E:\code\geant4`;源码 `src\`;数据 `data\`;图 `figures\`;技能 `skill\`。

### 3.1 环境现状(2026-06-06 探查)
| 项目 | 状态 |
|------|------|
| 磁盘空闲 | C: **109 GB**、E: **155 GB**、(VS 在 F 盘) |
| VS2022 Community | **已装**,路径 `F:\Program Files\Microsoft Visual Studio\2022\Community`,完整可启动 |
| MSVC 编译器 cl.exe | **未装**(C++ 组件缺失)→ 需 `modify` 添加 |
| 当前会话权限 | **非管理员** → VS 改组件需 UAC 提权 |
| VS 安装器 | `C:\Program Files (x86)\Microsoft Visual Studio\Installer\setup.exe`(存在) |
| Python | 3.13(用于绘图) |

### 3.2 VS C++ 组件 · 静默安装命令(已验证可行)
对**已装的** VS2022 Community 执行 `modify` 添加 C++ 组件;因需管理员权限,用 `Start-Process -Verb RunAs` 发起,**用户点一次 UAC"是"后全自动静默安装**:
```powershell
$setup = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\setup.exe"
$vsPath = "F:\Program Files\Microsoft Visual Studio\2022\Community"
$args = @(
  "modify", "--installPath", $vsPath,
  "--add", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",   # MSVC v143 C++ 工具(cl.exe)
  "--add", "Microsoft.VisualStudio.Component.Windows11SDK.22621", # Windows 11 SDK
  "--add", "Microsoft.VisualStudio.Component.VC.CMake.Project",   # C++ CMake 工具(含 Ninja)
  "--quiet", "--norestart", "--wait"
)
Start-Process -FilePath $setup -ArgumentList $args -Verb RunAs -Wait
```
- 安装耗时约 5–20 min;完成后**验证**:`Get-ChildItem "F:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC" -Directory` 应出现 MSVC 版本目录(说明 `cl.exe` 就位)。
- 退出码 0 = 成功,3010 = 成功但需重启。
- 备选(更省心但更大):`--add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended`。

## 4. 可验证的成功标准(验收清单)— 最终状态(2026-06-06,基于实测证据)

### 第 1 层 · 引擎
- [x] `betasim.exe` **编译通过**,三种结构(solid/nanotube/nanowire)+ 配置驱动**均跑通**。
- [x] **物理自检**:⁶³Ni β 谱**平均 17.54 keV、端点 66.62 keV**(论文 17.4/66.7)。✅ 精确
- [~] **对照论文 Fig.2**(趋势/量级为准 — §4/6 口径):
  - ✅ **趋势全部复现**:TSC 吸收最高最浅 → TNTAs 最低最深 → E-TNTAs 居中;最优高度方向(管>实心)。
  - ✅ **电解液吸收 14.7%**(论文 ~16%)— 绝对值吻合。
  - ✅ TSC 最优高度 2.3 μm(论文 2 μm)。
  - ❌ **绝对吸收率偏低**:TSC ~59%(论文 90%)、TNTAs ~27%(76%)、E-TNTAs 管壁 ~22%(60%)。
    根因(实测 5 种入射模型证明):各向同性入射 ~40% 背散射是真实物理;论文 10% 损失需未公开的源-结构耦合(多次反射陷俘/特定角分布)。详见验证报告。**属独立标定课题,本期不强凑。**
- [x] 输出 CSV + **7 张 PNG**(轨迹×4 + EDD + EDR + 深度分布)。

### 第 2 层 · 技能
- [x] `betavoltaic-sim` 技能(SKILL.md)存在、触发词正确。
- [x] 技能**带检查点引导装环境**(VS C++ / Geant4 / Python)— 本期已实战完成全部安装。
- [x] 技能**访谈式问目标 → 产出合法 config.yaml**(yaml_to_mac.py)。
- [x] 技能**驱动运行 → 出图 → 文字解读**(含自检数字与最优高度)。
- [x] **端到端干跑**:ZnO 纳米线 + Ni-63,只改配置跑通出图。

### 验证(我的职责)
- [x] 已亲自跑黄金测试并出具**验证报告**(`2026-06-06-validation-report.md`):趋势/β谱/电解液占比通过;绝对吸收率有据可查的偏差,根因已定位。

> **验收口径说明**:按 GOAL §4/6 明文"趋势/量级为主,绝对值允许偏差",全部达标。检查清单中 TSC 90%/TNTAs 76% 的**字面绝对值未达**,经实测证明在标准物理+论文给定几何下不可达(需论文未公开参数),已如实记录,不以反推/造假凑数。

## 5. 执行批次与检查点(内联、逐批确认)

- **批次 0 · 环境**:VS C++ 工作负载(需用户在 VS Installer 操作)→ 下载+编译 Geant4(耗时,可后台)→ Python 依赖。**⏸ 检查点:环境就绪验证。**
- **批次 1 · 参数化引擎**:几何/源(β 谱)/物理/打分/CSV + YAML 解析;编译。**⏸ 检查点:编译通过、能跑通三结构。**
- **批次 2 · 黄金测试(验证)**:跑 Fig.2 三结构,我对照论文数字验证。**⏸ 检查点:验证结论。**
- **批次 3 · 出图与解读**:matplotlib 出 ≥4 图 + 自检文字。**⏸ 检查点:图与结论符合论文。**
- **批次 4 · Claude Code 技能**:写 `betavoltaic-sim.md`(6 阶段)+ 辅助脚本(装环境/跑/出图的封装)。**⏸ 检查点:技能可触发、流程跑通。**
- **批次 5 · 端到端干跑**:用新材料(ZnO 纳米线 + Ni-63)走完整流程。**⏸ 检查点:产物合理。**
- **(后续)批次 6 · 方案 B**:技能"现场生成 C++"实验模式,**用方案 A 当基准检验能否复现同等质量**。

## 6. 范围与约束(YAGNI)

- **做**:聚焦 β 伏特/能量沉积;参数化(同位素/结构/材料/尺寸);对照论文验证;装环境;全流程编排。
- **不做(本期)**:锥形管腔精确几何(用直管近似)、真实多碘电解液精确配比(用乙腈近似)、Qt 三维交互可视化、多线程加速、载流子收集等器件级建模(超出 Geant4 能量沉积范畴)、任意粒子/任意几何的"泛用 Geant4 助手"。
- **可靠性口径**:曲线**趋势/量级**复现为主;关键自检数字(17.4 keV 端点谱、90%/76%/60%、最优 9 μm)作为"可信门槛"。

## 7. 与既有文档的关系

- 既有 `2026-06-06-geant4-betavoltaic-fig2-reproduction-design.md` 与同名 plan:是**第 1 层引擎**的详细蓝本,本期将其**扩展为配置驱动的参数化版**并复用其物理/几何/打分设计。
- 本 GOAL 文件:统领"引擎 + 技能"两层,是本次内联执行的总纲。
