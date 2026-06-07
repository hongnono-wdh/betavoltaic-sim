// BetaSpectrum.hh
// 纯 C++（不依赖 Geant4）的 β⁻ 连续能谱采样器，便于独立单元测试。
//
// 物理：允许跃迁（allowed transition）的 β⁻ 谱形
//   N(T) dT ∝ F(Z, T) · p · (T + m_e c²) · (Q - T)²  dT
// 其中
//   T            = β 电子动能
//   Q            = 跃迁端点动能（endpoint）
//   p c          = sqrt(T² + 2 T m_e c²)         （相对论动量×c）
//   m_e c²       = 510.998 keV
//   F(Z, T)      = 非相对论 Fermi 库仑修正函数（子核电荷 Z）
//                  F = 2πη / (1 - e^{-2πη}),  η = α Z / β,  β = v/c
//   对 β⁻：子核 Z 取正（⁶³Ni → ⁶³Cu，Z = 29），库仑吸引增强低能端，
//   把平均能量从 ~Q/3 拉低到实测 ~17.4 keV。
//
// 采样：在 [0, Q] 上对谱形做拒绝采样（rejection sampling）。
//
// 设计：把 RNG 作为可注入的可调用对象（默认 std::mt19937_64），
//       这样单元测试可用固定种子复现，Geant4 端可桥接 G4 的 RNG。

#ifndef BETA_SPECTRUM_HH
#define BETA_SPECTRUM_HH

#include <cmath>
#include <functional>
#include <random>
#include <string>
#include <stdexcept>

class BetaSpectrum {
public:
    // 已知 β⁻ 发射体参数表（端点动能 keV，子核电荷 Z）。
    struct IsotopeData {
        double endpoint_keV;   // Q
        int    daughterZ;      // 子核电荷（β⁻ 后 Z+1）
        double meanRef_keV;    // 文献平均能量（仅用于自检对照）
    };

    // 返回已知同位素参数；未知则抛异常。
    static IsotopeData lookup(const std::string& isotope) {
        // 端点/平均取自常用核数据（近似值，足够本仿真用）。
        if (isotope == "Ni-63") return {66.98, 29, 17.43};   // ⁶³Ni → ⁶³Cu
        if (isotope == "H-3")   return {18.59,  2,  5.68};   // ³H  → ³He
        if (isotope == "C-14")  return {156.5,  7, 49.47};   // ¹⁴C → ¹⁴N
        if (isotope == "Pm-147")return {224.6, 62, 61.93};   // ¹⁴⁷Pm → ¹⁴⁷Sm
        throw std::invalid_argument("BetaSpectrum: unknown isotope '" + isotope +
                                    "' (supported: Ni-63, H-3, C-14, Pm-147)");
    }

    static constexpr double kElectronMass_keV = 510.99895; // m_e c²
    static constexpr double kAlpha = 1.0 / 137.035999;     // 精细结构常数
    static constexpr double kPi = 3.14159265358979323846;  // 避免依赖 M_PI(MSVC 默认不定义)

    explicit BetaSpectrum(const std::string& isotope, unsigned long seed = 12345UL)
        : fData(lookup(isotope)), fGen(seed),
          fUniform(0.0, 1.0), fPeak(computePeak(fData)) {}

    // 允许外部注入 [0,1) 均匀随机源（桥接 Geant4 RNG）。
    void setUniformSource(std::function<double()> u) { fExternalUniform = std::move(u); }

    double endpoint_keV() const { return fData.endpoint_keV; }
    int    daughterZ()    const { return fData.daughterZ; }
    double meanRef_keV()  const { return fData.meanRef_keV; }

    // β 谱（未归一化）形状函数：shape(T)，T 单位 keV，T∈[0,Q]。
    static double shape(const IsotopeData& d, double T_keV) {
        if (T_keV <= 0.0 || T_keV >= d.endpoint_keV) return 0.0;
        const double mc2 = kElectronMass_keV;
        const double Etot = T_keV + mc2;
        const double pc = std::sqrt(T_keV * (T_keV + 2.0 * mc2)); // p·c
        const double q  = d.endpoint_keV - T_keV;                 // (Q - T)
        const double F  = fermiFunction(d.daughterZ, T_keV);
        return F * pc * Etot * q * q;
    }

    // 采样一个动能（keV）。
    double sampleKineticEnergy_keV() {
        const double Q = fData.endpoint_keV;
        for (int i = 0; i < 100000; ++i) {
            const double T = rand01() * Q;             // 候选动能
            const double y = rand01() * fPeak;         // 比较高度
            if (y <= shape(fData, T)) return T;
        }
        // 极不可能到这里；兜底返回平均量级值。
        return fData.meanRef_keV;
    }

private:
    // 非相对论 Fermi 库仑修正函数 F(Z, T)（β⁻，Z 为子核电荷取正）。
    static double fermiFunction(int Z, double T_keV) {
        if (Z == 0) return 1.0;
        const double mc2 = kElectronMass_keV;
        const double Etot = T_keV + mc2;
        const double beta = std::sqrt(T_keV * (T_keV + 2.0 * mc2)) / Etot; // v/c
        if (beta <= 1e-12) return 1.0;
        const double eta = kAlpha * Z / beta;          // β⁻：吸引，η>0
        const double x = 2.0 * kPi * eta;
        // F = x / (1 - e^{-x})，数值稳定处理。
        if (x < 1e-9) return 1.0;
        return x / (1.0 - std::exp(-x));
    }

    // 估计谱形峰值（用于拒绝采样的上包络），在 [0,Q] 上密集扫描取最大并留余量。
    static double computePeak(const IsotopeData& d) {
        double peak = 0.0;
        const int N = 4000;
        for (int i = 1; i < N; ++i) {
            const double T = d.endpoint_keV * i / static_cast<double>(N);
            peak = std::max(peak, shape(d, T));
        }
        return peak * 1.05; // 5% 余量，确保包络
    }

    double rand01() {
        if (fExternalUniform) return fExternalUniform();
        return fUniform(fGen);
    }

    IsotopeData fData;
    std::mt19937_64 fGen;
    std::uniform_real_distribution<double> fUniform;
    std::function<double()> fExternalUniform;
    double fPeak;
};

#endif // BETA_SPECTRUM_HH
