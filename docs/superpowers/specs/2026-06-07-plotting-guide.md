# 论文级能量沉积图 · 绘图方法与经验沉淀

> 目标:用本仿真画出可直接用于学术论文的能量沉积图(对照 Composites Part B 239(2022)109952 Fig.1/Fig.2)。每轮出图后自评、迭代。

## 1. 三类论文图(都来自同一次仿真)
| 图 | 脚本产物 | 含义 | 对应论文 |
|---|---|---|---|
| **2D 热图 + 双轴曲线** | `heatmap_<标签>.png` | 背景=(深度×横向)能量沉积密度热图;黑线=EDD(keV/μm);红线=EDR(%) | Fig.2 主面板 |
| **能量着色轨迹** | `trajcolor_<标签>.png` | X(横向) vs 深度,β 径迹点按电子动能着色(高=红→低=黄)+ 颜色条 | Fig.2 轨迹 |
| **复合面板** | `paper_panel_<标签>.png` | 上=轨迹,下=双轴 EDD/EDR | Fig.2 整体 |
| EDD/EDR/深度分布 | `EDD/EDR/DEPTH_DIST.png` | 多结构叠加分析曲线 | — |

引擎输出:`_edd_edr.csv`(含 `edd_kev_total`)、`_map2d.csv`(2D 热图)、`_tracks.csv`(含 `ke_kev`)、`_edep_points.csv`。

## 2. 关键经验(踩坑→正解)

### (A) ⭐ 论文 Fig.2 深度剖面用的是"单能 ~17.4 keV",不是完整连续 β 谱
- 实测:**完整 β 谱** → EDD 峰在表面、EDR@2μm 仅 60%(软尾表面沉积 + 硬尾深穿透,把剖面摊开)。
- **单能 17.4 keV** → EDD 峰次表面(~0.43μm)、**EDR@2μm=90%**、TNTAs 峰~2.4μm —— **精确匹配论文形状**。
- 推论:论文用平均能量当单能算 Fig.2。**画"论文同款图"用 `/gun/monoKeV 17.4`;要"更真实物理"用完整谱**(更准但剖面更散)。
- 这是"举一反三"的核心:别假设论文用了最完整的物理;先用对照数据反推它的简化。

### (B) 纵轴用 keV/μm,不要用"每箱 keV"或归一 a.u.
- 每箱 keV 依赖分箱宽度、数值偏小(~0.02);**keV/μm = 单位深度沉积强度**,不依赖分箱、量级与论文一致(TSC 峰 ~1.4 keV/μm)。
- 物理自检:∫EDD(keV/μm) d(depth) ≈ 每 β 沉积总能量 ≈ 0.9×平均能量。

### (C) 横向容纳必须收敛(否则 EDR 偏低,见验证报告)
- 默认 ±3μm 太小→高能 β 侧漏;Ni-63 用 `/det/lateralHalfUm 20`。

### (D) 热图配色用百分位 vmax(96),否则中线过亮把四周洗白
- `vmax=np.percentile(mat[mat>0], 96)`;`cmap="jet"`、`interpolation="bilinear"`。

### (E) 双轴 + 热图共用一个绘图框时,隐藏热图纵轴
- 热图纵向是横向 X(仅背景),`set_yticks([])`,只留左 keV / 右 % 两轴,避免标签重叠。

### (F) 轨迹按真实动能着色
- 引擎给 track 点输出 `ke_kev`;`cmap="autumn_r"`(低=黄,高=红),X 裁到 ±3μm。

## 3. 出"论文同款图"的推荐命令
```
/det/structure nanotube_array
/det/material  TiO2_anatase
/det/lateralHalfUm 20
/gun/isotope   Ni-63
/gun/monoKeV   17.4        # 论文同款:平均能量当单能;要真实物理则删掉此行用完整谱
/gun/incidence normal
/run/beamOn    40000
```
```
python analysis/plot.py --prefixes data/我的=标签   # 自动出 heatmap_/trajcolor_/paper_panel_
```

## 4. 仍可继续精修(下一轮候选)
- EDD 峰位 0.43μm vs 论文 0.9μm:与 TiO2 密度/buildup 有关,可微调材料密度核对。
- 热图色阶/平滑度、字体字号统一到期刊要求(如 7pt、矢量 PDF 输出)。
- 输出矢量 PDF(`fig.savefig(..., format='pdf')`)供排版。

## 5. 兼顾"好看 + 准确"——双轨对比图(compare_*)
单能(论文同款形状)与完整谱(准确数值)有取舍。解决:**一张图同时画两者**:
- 背景热图 + EDD/EDR **实线 = 完整 β 谱(准确,标注真实 EDR 饱和值)**;
- **虚线 = 单能 17.4 keV(论文同款次表面峰形状)**。
命令:`python analysis/plot.py --compare TSC:data/TSC_full:data/TSC_mono ...`
比单选一种更诚实、信息更丰富(还顺带解释了"论文为何峰在次表面")。

## 6. 图内文字一律用英文(否则中文→豆腐块 □□)
matplotlib 默认字体无 CJK 字形,中文标签会变方框;且国际期刊要求英文。所有 title/label/legend/annotation 用英文。μm/β 等符号 DejaVu Sans 可正常渲染。
