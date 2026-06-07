// DetectorConstruction.hh
// 按配置参数构建三类结构几何：solid / nanotube_array / nanowire_array，
// 可选管腔填充（电解液）与衬底。所有参数经 DetectorMessenger 由宏命令注入。
//
// 坐标约定：z 为深度方向；结构顶面在 z=0，结构向 -z 延伸（深度 d = -z）。
// ⁶³Ni 源板贴在结构上方（z∈[0, 源厚]）。

#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include <string>

class G4Material;
class G4LogicalVolume;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    enum class Structure { Solid, NanotubeArray, NanowireArray };

    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;

    // —— Messenger 设置接口（运行前由宏设置）——
    void SetStructure(const G4String& s);
    void SetMaterial(const G4String& m)  { fMaterialName = m; }
    void SetFill(const G4String& f)      { fFillName = f; }
    void SetSubstrate(const G4String& s) { fSubstrateName = s; }
    void SetHeightUm(G4double h)         { fHeightUm = h; }
    void SetTubeOuterNm(G4double r)      { fTubeOuterNm = r; }
    void SetTubeInnerNm(G4double r)      { fTubeInnerNm = r; }
    void SetPitchNm(G4double p)          { fPitchNm = p; }
    void SetArrayN(G4int n)              { fArrayN = n; }
    void SetLateralHalfUm(G4double v)    { fLateralHalfUm = v; }

    G4double HeightUm() const { return fHeightUm; }

private:
    G4Material* MakeMaterial(const G4String& name); // 材料工厂（按名）
    void BuildSolid(G4LogicalVolume* world);
    void BuildArray(G4LogicalVolume* world, bool hollow); // hollow=true 纳米管，false 纳米线

    DetectorMessenger* fMessenger = nullptr;

    Structure fStructure = Structure::NanotubeArray;
    G4String  fMaterialName  = "TiO2_anatase";
    G4String  fFillName      = "none";
    G4String  fSubstrateName = "Ti";

    G4double fHeightUm    = 12.0;
    G4double fTubeOuterNm = 60.0;
    G4double fTubeInnerNm = 50.0;
    G4double fPitchNm     = 120.0;
    G4int    fArrayN      = 8;

    G4double fSourceThickNm = 200.0; // ⁶³Ni 源板厚度

    // 横向半宽(um)：结构横向放大到 >> 电子射程，容纳侧向散射、避免侧漏伪影。
    // 源仅在中心 ±0.25um 发射，故 3um 足以容纳 β 侧向 straggling。
    G4double fLateralHalfUm = 3.0;
};

#endif // DETECTOR_CONSTRUCTION_HH
