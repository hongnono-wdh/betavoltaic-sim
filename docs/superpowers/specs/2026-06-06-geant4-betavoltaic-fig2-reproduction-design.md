# 设计文档：复现论文 Fig.2 的 Geant4 β 能量沉积仿真

- **日期**：2026-06-06
- **状态**：设计阶段（待用户最终评审）
- **对应论文**：Wang et al., *Betavoltaic-powered electrochemical cells using TiO₂ nanotube arrays incorporated with carbon nanotubes*, Composites Part B 239 (2022) 109952
- **复现目标**：论文 §2.1 / Fig.2 —— 用蒙特卡洛模拟 ⁶³Ni 发出的 β 粒子在三种结构（TSC / TNTAs / E-TNTAs）中的**轨迹**与**能量沉积分布（EDD/EDR vs 深度）**，并验证"TNTAs 最优高度 ≈ 9 μm"的结论。

---

## 1. 目标与成功标准

### 1.1 要做什么
1. 用**原生 C++ Geant4**（与论文同款工具）搭建仿真，物理用 **Livermore 低能电磁模型**。
2. 对三种结构各跑一次：
   - **TSC**：TiO₂ 实心立方体（solid cuboid）
   - **TNTAs**：TiO₂ 中空纳米管阵列
   - **E-TNTAs**：纳米管阵列 + 管内填充电解液
3. 产出三类图（对应 Fig.2）：
   - **轨迹图**：β 粒子径迹（红线）+ 能量沉积事件（黄点），三结构各一张
   - **EDD vs 深度**：三结构叠加对比
   - **EDR vs 深度**：三结构叠加对比，标注最优高度

### 1.2 可验证的成功标准（验收点）
- [ ] Geant4 原生 C++ 工程能在本机编译通过并运行。
- [ ] 采样得到的 β 能谱**平均能量 ≈ 17.4 keV、端点 ≈ 66.7 keV**（与论文一致，作为物理正确性自检）。
- [ ] EDD/EDR 曲线再现论文定性结论：
  - TSC：EDD 峰在 ~0.9 μm，**~90% 能量沉积在 2 μm 内**（约 10% 因背散射损失）。
  - TNTAs：EDD 谱更宽，**~76% 在 10 μm 深度内**。
  - E-TNTAs（管壁部分）：EDD 峰约在 2.0 μm，**~60% 在 9 μm 内**；电解液部分仅吸收约 16%。
  - 由 EDD/EDR 推出 **最优纳米管高度 ≈ 9 μm**。
- [ ] 输出 ≥ 4 张 PNG 图（轨迹×3 + EDD + EDR）保存在 `figures/`。

> 注：曲线**趋势/量级**复现为主，由于几何采用直管近似、电解液成分简化，绝对数值允许与论文有偏差。

---

## 2. 技术路线与工具链

### 2.1 路线决策（已与用户确认）
- **路线 ②：原生 C++ Geant4**（非 Python 绑定）。理由：天花板高、与论文/社区对齐、利于长期深入。
- **编译器：MSVC**（VS2022 补装 "Desktop development with C++"，落 C 盘）。
- **Geant4：源码编译**，安装与数据文件**全部放 E 盘**（C 盘空间紧张）。
- **几何：直管近似**（管腔不做锥形收缩）。
- **出图：Python + matplotlib**（仿真用 CSV 导出数据，绘图与仿真解耦）。

### 2.2 环境现状（已探查）
| 项目 | 状态 |
|------|------|
| OS | Windows 11 Pro for Workstations |
| VS2022 | 已装外壳，**C++ 工作负载未装**（需补装 MSVC v143 + Windows SDK） |
| CMake/Ninja | VS 自带（补装 C++ 工作负载后可用） |
| Python | 3.13.5（用于绘图） |
| Qt | 未装（不用交互式三维可视化） |
| C 盘空闲 | 11.7 GB（用户将先清理至 ≥20 GB） |
| E 盘空闲 | 155 GB（放 Geant4 全部内容） |

### 2.3 安装与构建步骤（概要，详细见实现计划）
1. VS Installer 补装 "Desktop development with C++"（MSVC + Windows SDK）。
2. 下载 Geant4 11.x 源码 → `E:\geant4\src`。
3. CMake 配置（关键选项）：
   - `-DCMAKE_INSTALL_PREFIX=E:\geant4\install`
   - `-DGEANT4_INSTALL_DATA=ON`（自动下载数据集到 E 盘）
   - `-DGEANT4_USE_QT=OFF`
   - `-DGEANT4_BUILD_MULTITHREADED=OFF`（单线程，简化打分逻辑）
   - 生成器：Ninja（或 "Visual Studio 17 2022"）
4. `cmake --build ... --config Release --target install`（耗时步骤，30 min–2 h）。
5. 运行前 source `E:\geant4\install\bin\geant4.(bat/ps1)` 设置数据集环境变量。
6. 编译本项目 C++ 应用（`find_package(Geant4)`）。

---

## 3. 物理建模

### 3.1 ⁶³Ni β 源
- 粒子：电子（e⁻）。
- 能谱：⁶³Ni 为允许跃迁（allowed transition），用 **Fermi β 谱理论**在端点 66.7 keV 下采样能量；自检平均能量 ≈ 17.4 keV。
  - 实现：`PrimaryGeneratorAction` 内预计算归一化累积分布，逐事件抽样（inverse-CDF）。
- 发射位置：在 ⁶³Ni 源板体积内（1000 × 1000 × 200 nm³）均匀采样 → 体分布可捕捉**源自吸收**与**背散射**。
- 发射方向：4π 各向同性（朝下进入结构的占比自然产生背散射损失，对应论文 TSC 的 ~10% 损失）。

### 3.2 物理列表
- 自定义 `G4VModularPhysicsList`，注册 **`G4EmLivermorePhysics`**（论文同款）。
- 产生阈（cuts）：设置较低的 range cut（约 10 nm）以在纳米尺度分辨能量沉积；`G4EmParameters` 下限能量设 ~100 eV。
- 单线程运行（`G4RunManagerFactory` Serial），打分逻辑无需线程合并。

---

## 4. 几何建模（直管近似）

坐标约定：z 轴为深度方向，结构顶面 z=0，向下深入（深度 d 增大）。横向 footprint ≈ 1 μm（匹配 1 μm² 源）。

| 体 | 材料 | 尺寸/参数 |
|----|------|-----------|
| World | G4_Galactic（真空） | ~4 × 4 × 20 μm³ box |
| ⁶³Ni 源板 | G4_Ni（ρ=8.9） | 1000 × 1000 × 200 nm³，贴结构顶面上方 |
| **TSC** 结构 | 锐钛矿 TiO₂（ρ≈3.78 g/cm³） | 1000 × 1000 × H nm 实心 box |
| **TNTAs** 结构 | 同上 TiO₂ | 8×8 中空管阵列：`G4Tubs`(rin=50 nm, rout=60 nm, h=H)，pitch=120 nm（管壁相切）；管内 + 管间为真空 |
| **E-TNTAs** 结构 | 管壁 TiO₂ + 管腔电解液 | 同 TNTAs 管阵列；每管腔内填 `G4Tubs`(rin=0, rout=50 nm, h=H) 电解液 |
| 电解液材料 | 近似乙腈 CH₃CN（ρ≈0.786 g/cm³） | 论文实际为乙腈基多碘电解液；简化为乙腈（等效密度介于 TNTAs 与 TSC 之间） |
| Ti 衬底（可选） | G4_Ti | 结构底面下方 ~1 μm，增强背散射真实性 |

- **结构高度 H = 12 μm**：高于论文最优值 9 μm，以便从 EDD/EDR 曲线中"定位"出最优高度。
- 锐钛矿 TiO₂ 用 `G4Material` 按 Ti:O=1:2 化学计量、密度 3.78 g/cm³ 自建。
- 结构类型通过 CLI 参数 / macro（`/det/structure TSC|TNTAs|ETNTAs`）切换，由 `DetectorMessenger` 控制。

---

## 5. 打分（Scoring）

`SteppingAction` 对每个 edep>0 的步：取步中点深度 z 与所在材料/部位，累加直方图。

### 5.1 EDD / EDR
- 深度分箱：12 μm 范围，240 箱（每箱 50 nm）。
- 对 E-TNTAs 区分两套累加：**管壁部分**与**电解液部分**（对应 Fig.2c / 2d）。
- 量定义：
  - **EDD(d)** = 该深度箱沉积能量 / 箱宽（单位 keV/μm），按总入射能量归一化便于跨结构比较。
  - **EDR(d)** = 深度 ≤ d 的累计沉积能量 / **进入结构的 β 总动能**（相对入射能量；故背散射损失使其饱和值 < 100%，再现"90%/76%/60%"叙述）。

### 5.2 轨迹捕捉
- 对前 N_traj 个事件（默认 1000，绘图时可下采样到 ~200 条以保持清晰）：
  - 记录每条径迹的折线点 (x, z)（投影到 x-z 平面，对应论文横截面视图）。
  - 记录 edep>0 的位置点 (x, z)（黄点 = 能量沉积事件）。

---

## 6. 数据输出与绘图

### 6.1 仿真输出（CSV，纯文本，无 ROOT 依赖）
- `data/<structure>_edd_edr.csv`：`depth_um, edd_total, edr_total`（E-TNTAs 额外含 `edd_wall, edd_elyte, edr_wall, edr_elyte`）。
- `data/<structure>_tracks.csv`：`event_id, x_um, z_um`（径迹折线点）。
- `data/<structure>_edep_points.csv`：`x_um, z_um, edep_kev`（沉积事件点）。

### 6.2 绘图脚本 `analysis/plot.py`（matplotlib）
- **图 A：轨迹图** —— 三结构各一张（红线径迹 + 黄点沉积 + 几何轮廓），版式贴近 Fig.2。
- **图 B：EDD vs 深度** —— TSC/TNTAs/E-TNTAs 叠加。
- **图 C：EDR vs 深度** —— 三结构叠加，标注最优高度 9 μm 及关键百分比。
- 自检输出：打印平均 β 能量、各结构关键百分比与推得的最优高度。
- 所有图保存到 `figures/`。

---

## 7. 工程结构

```
E:\code\geant4\
  CMakeLists.txt              # find_package(Geant4); 定义可执行文件
  src\
    main.cc
    DetectorConstruction.{hh,cc}    # 三结构几何（CLI/macro 切换）
    DetectorMessenger.{hh,cc}       # /det/structure 命令
    PhysicsList.{hh,cc}             # Livermore EM + cuts
    PrimaryGeneratorAction.{hh,cc}  # ⁶³Ni β 谱源
    SteppingAction.{hh,cc}          # EDD/EDR + 轨迹打分
    RunAction.{hh,cc}
    EventAction.{hh,cc}
    Analysis.{hh,cc}                # 直方图累加 + CSV 写出
  macros\
    run_tsc.mac  run_tnt.mac  run_etnt.mac  vis_off.mac
  analysis\
    plot.py
    requirements.txt           # numpy, matplotlib
  data\                        # 仿真输出（运行后生成）
  figures\                     # 最终 PNG
  build\                       # CMake 构建目录（忽略）
  doc\                         # 论文 PDF / markdown（已存在）
  README.md                    # 安装、构建、运行、出图全流程
```

---

## 8. 模块边界（各单元职责单一、接口清晰）

| 单元 | 职责 | 依赖 |
|------|------|------|
| `DetectorConstruction` | 按结构类型建几何与材料 | Geant4 几何；`DetectorMessenger` 传入类型 |
| `PhysicsList` | 注册 Livermore EM、设 cuts | Geant4 物理 |
| `PrimaryGeneratorAction` | ⁶³Ni β 谱采样与发射 | Geant4 事件；内置 β 谱表 |
| `SteppingAction` | 累加 EDD/EDR、记录轨迹 | `Analysis` |
| `Analysis` | 直方图聚合 + CSV 写出 | 仅标准库 |
| `plot.py` | 读 CSV 出图 | numpy/matplotlib（与 C++ 完全解耦） |

仿真（C++，产数据）与绘图（Python，消费数据）通过 **CSV 接口**解耦，可独立开发与调试。

---

## 9. 风险与应对

| 风险 | 应对 |
|------|------|
| C 盘空间不足以装 MSVC | 用户先清理至 ≥20 GB；Geant4 全量放 E 盘 |
| Geant4 源码编译耗时长/报错 | 用 Ninja 并行；关 Qt/MT 精简；编译日志逐项排查 |
| 纳米尺度步长 + 低能电子导致运行慢 | 单结构事件数可配置；EDD/EDR 用较多事件、轨迹图用少量事件 |
| 电解液成分简化致数值偏差 | 文档说明为近似；以趋势复现为验收口径 |
| 直管近似与论文锥形管腔差异 | 已与用户确认采用直管；必要时后续可升级为 `G4Cons` 锥台 |

---

## 10. 后续可扩展（本次不做，YAGNI）
- 锥形管腔（`G4Cons`）精确几何。
- 真实多碘电解液成分（LiI / I₂ / 4-tert-butylpyridine 等）精确配比与密度。
- Qt 交互式三维可视化。
- 多线程（MT）加速。
- 载流子分离/收集效率等器件级建模（超出 Geant4 能量沉积范畴）。
