// test_beta.cc
// 纯 C++ 单元测试（不依赖 Geant4）：验证 β 谱采样器的平均能量与端点，
// 对应 GOAL 的物理自检验收点（Ni-63 平均 ≈ 17.4 keV、端点 ≈ 66.7 keV）。
//
// 编译（环境就绪后，示例）：
//   cl /EHsc /O2 /I src test\test_beta.cc /Fe:build\test_beta.exe
//   或 g++ -O2 -I src test/test_beta.cc -o build/test_beta
// 运行：build\test_beta.exe

#include "BetaSpectrum.hh"
#include <cstdio>
#include <string>
#include <vector>

struct Case { std::string iso; double meanLo; double meanHi; double endpoint; };

int main() {
    // 验收容差：平均能量允许 ±1.5 keV（采样统计 + 简化 Fermi 函数近似）。
    std::vector<Case> cases = {
        {"Ni-63", 15.9, 18.9, 66.98},
        {"H-3",    4.8,  6.8, 18.59},
        {"C-14",  45.0, 54.0, 156.5},
    };

    const int N = 2000000;
    int failures = 0;

    for (const auto& c : cases) {
        BetaSpectrum bs(c.iso, 20260606UL);
        double sum = 0.0, maxT = 0.0;
        for (int i = 0; i < N; ++i) {
            double T = bs.sampleKineticEnergy_keV();
            sum += T;
            if (T > maxT) maxT = T;
        }
        double mean = sum / N;
        bool meanOk = (mean >= c.meanLo && mean <= c.meanHi);
        bool endOk  = (maxT <= c.endpoint + 0.5) && (maxT >= 0.7 * c.endpoint);
        std::printf("[%-6s] mean = %6.2f keV (expect %.1f-%.1f) %s | maxT = %6.2f keV (endpoint %.2f) %s\n",
                    c.iso.c_str(), mean, c.meanLo, c.meanHi, meanOk ? "OK" : "FAIL",
                    maxT, c.endpoint, endOk ? "OK" : "FAIL");
        if (!meanOk || !endOk) ++failures;
    }

    if (failures == 0) {
        std::printf("\nALL BETA-SPECTRUM CHECKS PASSED\n");
        return 0;
    }
    std::printf("\n%d CHECK(S) FAILED\n", failures);
    return 1;
}
