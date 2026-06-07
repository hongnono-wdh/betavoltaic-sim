# β 能量沉积仿真引擎 + Claude Code 技能

用 Geant4 模拟放射性同位素 β 源在纳米结构中的**能量沉积分布(EDD/EDR)、轨迹、最优结构高度**。
面向材料研究者:**只改一份 `config.yaml`,不写 C++**。配套 Claude Code 技能 `betavoltaic-sim` 全程引导。

> 复现对象:Wang et al., *Composites Part B* 239 (2022) 109952(TiO₂ 纳米管阵列 β 伏特电池,Fig.2)。

## 目录
```
src\          参数化 Geant4 引擎(C++,编译一次)
test\         纯 C++ β 谱单测
macros\       黄金测试宏(TSC/TNTAs/E-TNTAs)+ 默认宏
analysis\     plot.py(出图) / yaml_to_mac.py(配置转宏) / requirements.txt
configs\      config.yaml 示例(研究者只改这里)
scripts\      run_all.ps1 一键黄金测试
.claude\skills\betavoltaic-sim\  Claude Code 技能
docs\         设计文档 / GOAL / 实现计划
```

## 一、装环境(一次性)
1. **VS C++ 组件**(已装 VS2022 则静默添加,见 `docs\...GOAL.md` §3.2)。
2. **Geant4 11.x** 源码编译装到 `E:\geant4\install`(`-DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_QT=OFF`)。
3. **Python 库**:`pip install -r analysis\requirements.txt`。

## 二、构建引擎
```powershell
. .\setup_env.ps1                       # 载入 Geant4 环境
cmake -S . -B build -G "Ninja"
cmake --build build
.\build\test_beta.exe                   # 自检:Ni-63 平均≈17.4keV
```

## 三、跑黄金测试(复现论文 Fig.2)
```powershell
.\scripts\run_all.ps1                    # 三结构 + 出图;图见 figures\
```

## 四、跑自己的材料(改配置即可)
```powershell
# 1) 复制一份 configs\example_*.yaml 改成你的设置
# 2) 转宏并运行
python analysis\yaml_to_mac.py configs\my.yaml -o run.mac
.\build\betasim.exe run.mac
# 3) 出图
python analysis\plot.py --prefixes data\my=MyRun
```
或者直接对 Claude 说"帮我跑一个 ZnO 纳米线 + Ni-63 的能量沉积仿真",技能会全程带你走。

## 验收口径
β 谱平均 17.4keV;EDR 量级 TSC~90%@2μm、TNTAs~76%@10μm、E-TNTAs 管壁~60%@9μm、电解液~16%;最优高度≈9μm。趋势/量级为准(直管+乙腈近似)。
