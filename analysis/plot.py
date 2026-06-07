#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
plot.py — 读取 betasim 导出的 CSV，生成论文 Fig.2 风格图：
  1) 轨迹图（每结构一张）：几何轮廓 + β 径迹(红) + 能量沉积点(黄)
  2) EDD vs 深度（多结构叠加）
  3) EDR vs 深度（多结构叠加，标注最优高度 ~9um 与关键百分比）

用法：
  python plot.py                       # 默认画黄金测试三结构 (TSC/TNTAs/ETNTAs)
  python plot.py --prefixes data/myrun=MyRun
  python plot.py --data-dir data --out-dir figures --prefixes data/TSC=TSC data/TNTAs=TNTAs

与仿真通过 CSV 解耦，仅依赖 numpy + matplotlib。
"""
import argparse
import os
import sys
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt


def read_edd_edr(prefix):
    path = prefix + "_edd_edr.csv"
    if not os.path.exists(path):
        return None
    data = np.genfromtxt(path, delimiter=",", names=True)
    return data


def read_points(path):
    if not os.path.exists(path):
        return None
    if os.path.getsize(path) < 20:
        return None
    return np.genfromtxt(path, delimiter=",", names=True)


def pct_at(data, depth_um, col="edr_total"):
    """返回累计沉积比(%) @ 给定深度。"""
    d = data["depth_um"]
    idx = np.searchsorted(d, depth_um)
    idx = min(idx, len(d) - 1)
    return data[col][idx] * 100.0


def plot_trajectory(prefix, label, out_dir, height_um=12.0):
    tracks = read_points(prefix + "_tracks.csv")
    edeps = read_points(prefix + "_edep_points.csv")
    if tracks is None:
        print(f"[skip trajectory] no tracks for {label}")
        return

    fig, ax = plt.subplots(figsize=(5, 6))

    # 几何带：源(灰) + 结构(浅)
    ax.axhspan(0, 0.2, color="0.6", alpha=0.5, label="$^{63}$Ni source")
    ax.axhspan(-height_um, 0, color="0.85", alpha=0.6, label=f"{label} structure")

    # 轨迹：按 event 连线，跳变过大则断开（区分不同 track）
    ev = tracks["event_id"].astype(int)
    x = tracks["x_um"]
    z = tracks["z_um"]
    for e in np.unique(ev):
        m = ev == e
        xs, zs = x[m], z[m]
        if len(xs) < 2:
            ax.plot(xs, zs, color="red", lw=0.4, alpha=0.5)
            continue
        # 断点：相邻点距离过大
        dist = np.hypot(np.diff(xs), np.diff(zs))
        breaks = np.where(dist > 0.5)[0]
        start = 0
        for b in list(breaks) + [len(xs) - 1]:
            ax.plot(xs[start:b + 1], zs[start:b + 1], color="red", lw=0.4, alpha=0.5)
            start = b + 1

    # 沉积点(黄)
    if edeps is not None:
        ax.scatter(edeps["x_um"], edeps["z_um"], s=2, color="gold",
                   alpha=0.6, label="energy deposition", zorder=5)

    ax.set_xlabel("x (μm)")
    ax.set_ylabel("z / depth (μm)")
    ax.set_title(f"β trajectories in {label}")
    ax.set_xlim(-1.2, 1.2)
    ax.set_ylim(-height_um - 1, 0.6)
    ax.legend(loc="lower right", fontsize=7)
    fig.tight_layout()
    out = os.path.join(out_dir, f"trajectory_{label}.png")
    fig.savefig(out, dpi=150)
    plt.close(fig)
    print(f"[ok] {out}")


def read_map2d(prefix):
    """读 _map2d.csv -> (matrix[ndepth,nx], meta dict)。"""
    path = prefix + "_map2d.csv"
    if not os.path.exists(path):
        return None, None
    with open(path, "r", encoding="utf-8") as f:
        header = f.readline()
    meta = {}
    for tok in header.replace("#", "").split():
        if "=" in tok:
            k, v = tok.split("=")
            meta[k] = float(v)
    mat = np.genfromtxt(path, delimiter=",", skip_header=1)
    if mat.ndim == 1:
        mat = mat.reshape(1, -1)
    return mat, meta


def plot_traj_colored(prefix, label, out_dir, max_depth=10.0, x_half=3.0):
    """论文图1风格:X(横向) vs 深度,β 径迹点按电子动能着色(高=红→低=黄),带颜色条。"""
    tracks = read_points(prefix + "_tracks.csv")
    if tracks is None or "ke_kev" not in tracks.dtype.names:
        print(f"[skip traj-colored] no track-energy for {label}")
        return
    depth = -tracks["z_um"]
    x = tracks["x_um"]
    ke = tracks["ke_kev"]
    m = (depth >= 0) & (depth <= max_depth) & (np.abs(x) <= x_half)
    fig, ax = plt.subplots(figsize=(6, 3.0))
    sc = ax.scatter(depth[m], x[m], c=ke[m], s=1.2, cmap="autumn_r",
                    vmin=0, vmax=np.percentile(ke[m], 97) if m.any() else 67,
                    linewidths=0)
    ax.set_xlim(0, max_depth); ax.set_ylim(-x_half, x_half)
    ax.set_xlabel("Height / Depth (μm)")
    ax.set_ylabel("X (μm)")
    ax.set_title(f"β trajectories (colored by energy) — {label}", fontsize=10)
    cb = fig.colorbar(sc, ax=ax, pad=0.02); cb.set_label("electron energy (keV)")
    fig.tight_layout()
    out = os.path.join(out_dir, f"trajcolor_{label}.png")
    fig.savefig(out, dpi=150); plt.close(fig)
    print(f"[ok] {out}")


def plot_heatmap_panel(prefix, label, out_dir, max_depth=10.0):
    """论文图2风格:2D 沉积热图(背景)+ 黑线 EDD(keV左轴)+ 红线 EDR(%右轴)。"""
    mat, meta = read_map2d(prefix)
    data = read_edd_edr(prefix)
    if mat is None or data is None:
        print(f"[skip heatmap] no map2d/edd for {label}")
        return
    xhalf = meta.get("xHalfUm", 2.5)
    ndepth = mat.shape[0]
    bw = meta.get("binWidthUm", 0.05)
    full_depth = ndepth * bw

    fig, axh = plt.subplots(figsize=(6, 4.2))
    # 热图:行=深度(转置使深度在 x 轴),列=横向 x。百分位配色,避免中线过亮把四周洗白。
    extent = [0, full_depth, -xhalf, xhalf]
    nz = mat[mat > 0]
    vmax = np.percentile(nz, 96) if nz.size else 1.0
    axh.imshow(mat.T, origin="lower", aspect="auto", extent=extent,
               cmap="jet", interpolation="bilinear", vmin=0, vmax=vmax)
    axh.set_xlim(0, max_depth)
    axh.set_yticks([])               # 热图纵向=横向X,仅作背景;隐藏,避免与 keV 轴标签重叠
    axh.set_xlabel("Height (μm)")

    # 叠加曲线:用独立坐标轴(共享 x)
    pos = axh.get_position()
    axE = fig.add_axes([pos.x0, pos.y0, pos.width, pos.height])
    axE.set_facecolor("none")
    axE.set_xlim(0, max_depth)
    d = data["depth_um"]
    # EDD:keV/μm(单位深度沉积强度,每入射β)——不依赖分箱宽度,量级与论文一致
    if "edd_kev_total" in data.dtype.names and len(d) > 1:
        bw = d[1] - d[0]
        edd = data["edd_kev_total"] / bw
        ylab = "Energy deposition (keV/μm)"
    else:
        edd = data["edd_total"]; ylab = "Energy deposition (a.u.)"
    axE.plot(d, edd, color="black", lw=2)
    axE.set_ylim(0, edd.max() * 1.15 if edd.max() > 0 else 1)
    axE.set_ylabel(ylab)
    axE.patch.set_alpha(0)
    axE.set_xticks([])

    axR = axE.twinx()
    axR.plot(d, data["edr_total"] * 100.0, color="red", lw=2)
    axR.set_ylim(0, 100)
    axR.set_ylabel("Deposition ratio (%)", color="red")
    axR.tick_params(axis="y", labelcolor="red")

    axh.set_title(f"2D deposition map + EDD/EDR — {label}", fontsize=10)
    out = os.path.join(out_dir, f"heatmap_{label}.png")
    fig.savefig(out, dpi=150); plt.close(fig)
    print(f"[ok] {out}")


def plot_paper_panel(prefix, label, out_dir, max_depth=10.0):
    """论文 Fig.2 风格复合面板:上=β轨迹,下=双轴(红 EDD / 蓝 EDR) vs 深度。"""
    data = read_edd_edr(prefix)
    if data is None:
        print(f"[skip paper-panel] no data for {label}")
        return
    edeps = read_points(prefix + "_edep_points.csv")

    fig = plt.figure(figsize=(5.2, 6.2))
    gs = fig.add_gridspec(2, 1, height_ratios=[1.0, 1.25], hspace=0.08)

    # —— 上:轨迹/沉积点(深度横轴,横向纵轴),与下图共享深度轴 —— #
    ax0 = fig.add_subplot(gs[0])
    if edeps is not None:
        depth_pts = -edeps["z_um"]
        ax0.scatter(depth_pts, edeps["x_um"], s=2, c="orangered", alpha=0.5, linewidths=0)
    ax0.set_xlim(0, max_depth)
    ax0.set_ylabel("x (μm)")
    ax0.set_title(f"β trajectories & energy deposition — {label}", fontsize=10)
    ax0.tick_params(labelbottom=False)

    # —— 下:双轴 EDD(红,左)/ EDR(蓝,右)—— #
    ax1 = fig.add_subplot(gs[1], sharex=ax0)
    d = data["depth_um"]
    if "edd_kev_total" in data.dtype.names and len(d) > 1:
        edd_y = data["edd_kev_total"] / (d[1] - d[0])
        edd_lab = "Deposition energy (keV/μm)"
    else:
        edd_y = data["edd_total"]; edd_lab = "Deposition energy (a.u.)"
    ax1.plot(d, edd_y, color="red", lw=2, label="EDD")
    ax1.set_xlabel("Depth (μm)")
    ax1.set_ylabel(edd_lab, color="red")
    ax1.tick_params(axis="y", labelcolor="red")
    ax1.set_xlim(0, max_depth)

    ax2 = ax1.twinx()
    ax2.plot(d, data["edr_total"] * 100.0, color="blue", lw=2, label="EDR")
    ax2.set_ylabel("Deposition ratio (%)", color="blue")
    ax2.tick_params(axis="y", labelcolor="blue")
    ax2.set_ylim(0, 100)

    fig.tight_layout()
    out = os.path.join(out_dir, f"paper_panel_{label}.png")
    fig.savefig(out, dpi=150)
    plt.close(fig)
    print(f"[ok] {out}")


def plot_edd(series, out_dir):
    fig, ax = plt.subplots(figsize=(6, 4.5))
    for prefix, label, data in series:
        ax.plot(data["depth_um"], data["edd_total"], lw=1.5, label=label)
    ax.set_xlabel("depth (μm)")
    ax.set_ylabel("EDD  (fraction / μm)")
    ax.set_title("Energy Deposition Density vs depth")
    ax.legend()
    ax.grid(alpha=0.3)
    fig.tight_layout()
    out = os.path.join(out_dir, "EDD.png")
    fig.savefig(out, dpi=150)
    plt.close(fig)
    print(f"[ok] {out}")


def optimal_height(data, frac=0.90):
    """返回沉积能量累计达到 total*frac 的深度(um) = 推荐结构高度。"""
    depth = data["depth_um"]
    edd = data["edd_total"]
    cum = np.cumsum(edd)
    total = cum[-1] if cum[-1] > 0 else 1.0
    target = total * frac
    idx = int(np.searchsorted(cum, target))
    idx = min(idx, len(depth) - 1)
    return depth[idx]


def plot_depth_distribution(series, out_dir):
    """沉积能量归一化深度分布(累计/总沉积),直接给出最优高度。"""
    fig, ax = plt.subplots(figsize=(6, 4.5))
    for prefix, label, data in series:
        cum = np.cumsum(data["edd_total"])
        total = cum[-1] if cum[-1] > 0 else 1.0
        ax.plot(data["depth_um"], cum / total * 100.0, lw=1.5, label=label)
        h90 = optimal_height(data, 0.90)
        ax.axvline(h90, ls=":", lw=0.8, alpha=0.5)
    ax.axhline(90, color="k", ls="--", lw=0.8, alpha=0.5)
    ax.text(0.2, 91, "90% of deposited", fontsize=8)
    ax.set_xlabel("depth (μm)")
    ax.set_ylabel("cumulative deposited energy (% of total)")
    ax.set_title("Deposition depth distribution → optimal height")
    ax.legend()
    ax.grid(alpha=0.3)
    fig.tight_layout()
    out = os.path.join(out_dir, "DEPTH_DIST.png")
    fig.savefig(out, dpi=150)
    plt.close(fig)
    print(f"[ok] {out}")


def plot_edr(series, out_dir):
    fig, ax = plt.subplots(figsize=(6, 4.5))
    for prefix, label, data in series:
        ax.plot(data["depth_um"], data["edr_total"] * 100.0, lw=1.5, label=label)
    ax.axvline(9.0, color="k", ls="--", lw=1, alpha=0.6)
    ax.text(9.1, 5, "optimum ≈ 9 μm", rotation=90, va="bottom", fontsize=8)
    ax.set_xlabel("depth (μm)")
    ax.set_ylabel("EDR  (cumulative %)")
    ax.set_title("Energy Deposition Ratio vs depth")
    ax.legend()
    ax.grid(alpha=0.3)
    fig.tight_layout()
    out = os.path.join(out_dir, "EDR.png")
    fig.savefig(out, dpi=150)
    plt.close(fig)
    print(f"[ok] {out}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--data-dir", default="data")
    ap.add_argument("--out-dir", default="figures")
    ap.add_argument("--height-um", type=float, default=12.0)
    ap.add_argument("--prefixes", nargs="*", default=None,
                    help="形如 data/TSC=TSC ；省略则用黄金测试默认三结构")
    args = ap.parse_args()

    os.makedirs(args.out_dir, exist_ok=True)

    if args.prefixes:
        items = []
        for p in args.prefixes:
            if "=" in p:
                pre, lab = p.split("=", 1)
            else:
                pre, lab = p, os.path.basename(p)
            items.append((pre, lab))
    else:
        items = [
            (os.path.join(args.data_dir, "TSC"), "TSC"),
            (os.path.join(args.data_dir, "TNTAs"), "TNTAs"),
            (os.path.join(args.data_dir, "ETNTAs"), "ETNTAs"),
        ]

    series = []
    for prefix, label in items:
        data = read_edd_edr(prefix)
        if data is None:
            print(f"[warn] missing {prefix}_edd_edr.csv — skipped")
            continue
        series.append((prefix, label, data))
        plot_trajectory(prefix, label, args.out_dir, args.height_um)
        plot_paper_panel(prefix, label, args.out_dir)
        plot_traj_colored(prefix, label, args.out_dir)
        plot_heatmap_panel(prefix, label, args.out_dir)

    if not series:
        print("ERROR: no data found. Run betasim first.")
        return 1

    plot_edd(series, args.out_dir)
    plot_edr(series, args.out_dir)
    plot_depth_distribution(series, args.out_dir)

    # —— 自检摘要（对照论文 Fig.2 定性结论）——
    print("\n===== EDR self-check vs paper Fig.2 =====")
    for prefix, label, data in series:
        line = f"{label:8s}: "
        line += f"@2μm={pct_at(data,2.0):5.1f}%  "
        line += f"@9μm={pct_at(data,9.0):5.1f}%  "
        line += f"@10μm={pct_at(data,10.0):5.1f}%"
        if "edr_fill" in data.dtype.names and data["edr_fill"].max() > 0:
            line += f"  fill@5μm={pct_at(data,5.0,'edr_fill'):5.1f}%"
        line += f"  | optimal_height(90%)={optimal_height(data):.1f}μm"
        print(line)
    print("paper: TSC~90%@2μm | TNTAs~76%@10μm | ETNTAs wall~60%@9μm, fill~16%")
    print("=========================================")
    return 0


if __name__ == "__main__":
    sys.exit(main())
