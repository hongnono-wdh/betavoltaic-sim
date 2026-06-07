#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
yaml_to_mac.py — 把研究者友好的 config.yaml 转成 Geant4 宏 run.mac。
这样 C++ 引擎零 YAML 依赖，用户端仍是易懂的 YAML。

用法:
  python yaml_to_mac.py config.yaml -o run.mac

config.yaml 示例见 configs/ 目录。
"""
import argparse
import sys
import os

try:
    import yaml
except ImportError:
    print("需要 pyyaml: pip install pyyaml", file=sys.stderr)
    sys.exit(2)

VALID_STRUCTURE = {"solid", "nanotube_array", "nanowire_array"}
VALID_ISOTOPE = {"Ni-63", "H-3", "C-14", "Pm-147"}


def err(msg):
    print(f"[config 错误] {msg}", file=sys.stderr)
    sys.exit(1)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("config")
    ap.add_argument("-o", "--out", default="run.mac")
    args = ap.parse_args()

    with open(args.config, "r", encoding="utf-8") as f:
        cfg = yaml.safe_load(f)

    # —— 校验关键字段 —— #
    structure = cfg.get("structure", "nanotube_array")
    if structure not in VALID_STRUCTURE:
        err(f"structure 必须是 {VALID_STRUCTURE}，得到 '{structure}'")
    isotope = cfg.get("isotope", "Ni-63")
    if isotope not in VALID_ISOTOPE:
        err(f"isotope 必须是 {VALID_ISOTOPE}，得到 '{isotope}'")

    incidence = cfg.get("incidence", "isotropic")  # isotropic | hemisphere | normal
    if incidence not in {"isotropic", "hemisphere", "normal"}:
        err(f"incidence 必须是 isotropic/hemisphere/normal，得到 '{incidence}'")
    material = cfg.get("material", "TiO2_anatase")
    fill = cfg.get("fill", "none")
    substrate = cfg.get("substrate", "Ti")
    geo = cfg.get("geometry", {}) or {}
    height = geo.get("height_um", 12)
    tube_out = geo.get("tube_outer_nm", 60)
    tube_in = geo.get("tube_inner_nm", 50)
    pitch = geo.get("pitch_nm", 120)
    array = geo.get("array", "8x8")
    arrayN = int(str(array).lower().split("x")[0])
    run = cfg.get("run", {}) or {}
    n_events = run.get("n_events", 50000)
    n_tracks = run.get("n_tracks", 150)
    max_depth = run.get("max_depth_um", max(15, height + 3))
    prefix = cfg.get("output_prefix", "data/myrun")

    # 确保输出目录存在
    out_data_dir = os.path.dirname(prefix)
    if out_data_dir:
        os.makedirs(out_data_dir, exist_ok=True)

    lines = [
        f"# 由 yaml_to_mac.py 从 {os.path.basename(args.config)} 自动生成",
        f"/det/structure {structure}",
        f"/det/material  {material}",
        f"/det/fill      {fill}",
        f"/det/substrate {substrate}",
        f"/det/heightUm  {height}",
        f"/det/tubeOuterNm {tube_out}",
        f"/det/tubeInnerNm {tube_in}",
        f"/det/pitchNm   {pitch}",
        f"/det/arrayN    {arrayN}",
        f"/gun/isotope   {isotope}",
        f"/gun/incidence {incidence}",
        f"/ana/prefix    {prefix}",
        f"/ana/nTracks   {n_tracks}",
        f"/ana/maxDepthUm {max_depth}",
        "/run/initialize",
        f"/run/beamOn    {n_events}",
        "",
    ]
    with open(args.out, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))

    print(f"[ok] 生成 {args.out}")
    print(f"     结构={structure} 材料={material} 填充={fill} 同位素={isotope}")
    print(f"     高度={height}um 阵列={arrayN}x{arrayN} 事件={n_events}")
    print(f"     输出前缀={prefix}")


if __name__ == "__main__":
    main()
