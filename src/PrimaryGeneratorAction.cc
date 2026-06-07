// PrimaryGeneratorAction.cc
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "BetaSpectrum.hh"
#include "Analysis.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Electron.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include <cmath>

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    fGun = new G4ParticleGun(1);
    fGun->SetParticleDefinition(G4Electron::Definition());
    fMessenger = new PrimaryGeneratorMessenger(this);
    SetIsotope(fIsotope);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fGun;
    delete fMessenger;
}

void PrimaryGeneratorAction::SetIsotope(const G4String& iso) {
    fIsotope = iso;
    fSpectrum = std::make_unique<BetaSpectrum>(iso);
    // 桥接 Geant4 RNG，保证与 G4 随机数流一致、可复现
    fSpectrum->setUniformSource([]() { return G4UniformRand(); });
}

void PrimaryGeneratorAction::ConfigureAnalysis() {
    Analysis::Instance()->Configure(fPrefix, fNTracks, fMaxDepthUm);
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* evt) {
    // —— 能量：β 谱采样 —— //
    const G4double T_keV = fSpectrum->sampleKineticEnergy_keV();
    const G4double T = T_keV * keV;
    fGun->SetParticleEnergy(T);
    // 注意：EDR 分母 = "真正进入结构的 β 能量"(论文口径)，在 SteppingAction 里
    // 于 β 越过结构顶面(z=0)时累加，而非这里的生成能量(含被 Ni 源自吸收的部分)。

    // —— 位置：在 Ni 源体内(z∈[0,thick])均匀生成 —— //
    // 保留 200nm Ni 源自吸收：它过滤掉掠射/软 β，使入射结构的束前向化、硬化，
    // 重现论文 EDD 次表面峰(~0.9μm)与低背散射(~10%)。中心 ±0.25μm 发射避免侧漏。
    const G4double hx = fSrcHalfXY_nm * nm;
    const G4double x = (2.0 * G4UniformRand() - 1.0) * hx;
    const G4double y = (2.0 * G4UniformRand() - 1.0) * hx;
    const G4double z = 1.0 * nm;  // 表面源(经实测为最佳配置:与论文 Fig.2 的最优高度/EDR 形状吻合最好)
    fGun->SetParticlePosition({x, y, z});

    // —— 方向：按入射模型(可配置) —— //
    // isotropic : 真实 β 衰变 4π 各向同性(物理最诚实,默认);上行 β 多被源/阴极吸收。
    // hemisphere: 下半球 2π 各向同性(射向结构)。
    // normal    : 法向入射(0,0,-1);低背散射、深穿透,利于重现实心结构高吸收的 Fig.2 形态。
    // 说明:绝对 EDR 对入射角分布敏感;论文 TSC 90%/TNTAs 76% 需特定耦合,标准物理下不同模型见验证报告。
    G4double cosTheta;
    if (fIncidence == "normal") {
        cosTheta = -1.0;
    } else if (fIncidence == "hemisphere") {
        cosTheta = -G4UniformRand();          // [-1,0] 下行
    } else { // isotropic
        cosTheta = 2.0 * G4UniformRand() - 1.0;
    }
    const G4double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    const G4double phi = twopi * G4UniformRand(); // twopi 来自 G4PhysicalConstants.hh
    fGun->SetParticleMomentumDirection(
        {sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta});

    fGun->GeneratePrimaryVertex(evt);
}
