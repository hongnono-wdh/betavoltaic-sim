#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
compare_runs.py — 方案 B 验证工具:把"现场生成的引擎(方案 B)"输出的 CSV
与"验证过的方案 A 引擎"输出的 CSV 逐深度比较,判定 B 是否复现了 A 的质量。

用法:
  python compare_runs.py A_prefix B_prefix [--tol 0.05]
  例: python compare_runs.py data/TSC dataB/TSC --tol 0.08

判定标准(可调):
  - EDR 饱和值相对误差 < tol
  - EDD 曲线形状(峰深度)接近
  - 90% 沉积深度(最优高度)相对误差 < tol
返回码 0=通过, 1=不通过, 便于自动化/CI。
"""
import argparse, sys, os
import numpy as np


def load(prefix):
    p = prefix + "_edd_edr.csv"
    if not os.path.exists(p):
        print(f"[err] missing {p}", file=sys.stderr); sys.exit(2)
    return np.genfromtxt(p, delimiter=",", names=True)


def optimal_height(d, frac=0.90):
    cum = np.cumsum(d["edd_total"]); tot = cum[-1] if cum[-1] > 0 else 1.0
    return d["depth_um"][min(int(np.searchsorted(cum, tot*frac)), len(d)-1)]


def relerr(a, b):
    denom = max(abs(a), 1e-12)
    return abs(a - b) / denom


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("a_prefix"); ap.add_argument("b_prefix")
    ap.add_argument("--tol", type=float, default=0.08)
    args = ap.parse_args()

    A, B = load(args.a_prefix), load(args.b_prefix)

    edrA = A["edr_total"][-1] * 100; edrB = B["edr_total"][-1] * 100
    hA, hB = optimal_height(A), optimal_height(B)
    # EDD 峰深度
    pkA = A["depth_um"][int(np.argmax(A["edd_total"]))]
    pkB = B["depth_um"][int(np.argmax(B["edd_total"]))]

    print(f"{'metric':22s} {'A(ref)':>10s} {'B(gen)':>10s} {'relerr':>8s}")
    print(f"{'EDR saturation %':22s} {edrA:10.2f} {edrB:10.2f} {relerr(edrA,edrB):8.3f}")
    print(f"{'optimal height um':22s} {hA:10.2f} {hB:10.2f} {relerr(hA,hB):8.3f}")
    print(f"{'EDD peak depth um':22s} {pkA:10.3f} {pkB:10.3f}")

    ok = relerr(edrA, edrB) < args.tol and relerr(hA, hB) < args.tol
    print("\nRESULT:", "PASS (B reproduces A)" if ok else "FAIL (B differs from A)")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
