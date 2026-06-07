// PrimaryGeneratorAction.hh
// ⁶³Ni（或其他 β⁻ 发射体）源：在源板体积内均匀发射、4π 各向同性，
// 能量按 BetaSpectrum 采样；同时把入射动能累加到 Analysis（EDR 分母）。
#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include <memory>

class G4ParticleGun;
class G4Event;
class BetaSpectrum;
class PrimaryGeneratorMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event* evt) override;

    void SetIsotope(const G4String& iso);
    void SetIncidence(const G4String& mode) { fIncidence = mode; } // isotropic|hemisphere|normal
    void SetEminKeV(G4double e)            { fEminKeV = e; }       // 有效源能量下限(模拟源对软β的自吸收过滤)
    void SetSourceMode(const G4String& m)  { fSourceMode = m; }    // surface(贴顶面) | volume(Ni 源体内,含自吸收)
    void SetMonoKeV(G4double e)            { fMonoKeV = e; }        // >0 时用单能电子(校准基准用),否则用 β 谱
    void SetAnaPrefix(const G4String& p)   { fPrefix = p; }
    void SetAnaNTracks(G4int n)            { fNTracks = n; }
    void SetAnaMaxDepthUm(G4double d)      { fMaxDepthUm = d; }
    void ConfigureAnalysis();              // 把 prefix/nTracks/maxDepth 推给 Analysis

private:
    G4ParticleGun*                fGun = nullptr;
    std::unique_ptr<BetaSpectrum> fSpectrum;
    PrimaryGeneratorMessenger*    fMessenger = nullptr;

    G4String fIsotope = "Ni-63";
    G4String fIncidence = "isotropic"; // 入射角分布:isotropic(真实4π) | hemisphere(2π下行) | normal(法向)
    G4double fEminKeV = 0.0;            // 有效源能量下限(keV);>0 时丢弃更软的 β,模拟源自吸收硬化
    G4String fSourceMode = "surface";  // surface(贴结构顶面) | volume(Ni 源体内均匀,含自吸收)
    G4double fMonoKeV = 0.0;            // >0:单能电子(校准 η/射程 用);0:用 β 谱
    G4String fPrefix = "run";
    G4int    fNTracks = 200;
    G4double fMaxDepthUm = 15.0;

    // 源在结构顶面中心小区域发射(±fSrcHalfXY_nm)，远离侧壁以避免侧向泄漏伪影；
    // 结构横向被放大到 >> 电子射程来容纳侧向散射(见 DetectorConstruction)。
    G4double fSrcHalfXY_nm = 250.0;
    G4double fSrcThick_nm  = 200.0;
};

#endif // PRIMARY_GENERATOR_ACTION_HH
