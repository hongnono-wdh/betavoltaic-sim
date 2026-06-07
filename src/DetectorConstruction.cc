// DetectorConstruction.cc
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RunManager.hh"
#include <algorithm>

DetectorConstruction::DetectorConstruction() {
    fMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction() {
    delete fMessenger;
}

void DetectorConstruction::SetStructure(const G4String& s) {
    if      (s == "solid")          fStructure = Structure::Solid;
    else if (s == "nanotube_array") fStructure = Structure::NanotubeArray;
    else if (s == "nanowire_array") fStructure = Structure::NanowireArray;
    else {
        G4cerr << "[DetectorConstruction] unknown structure '" << s
               << "', keeping previous." << G4endl;
        return;
    }
    // 几何已变，通知 RunManager 重建。
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

// —— 材料工厂：按名字返回材料；先查内置自建材料，再回退到 NIST —— //
G4Material* DetectorConstruction::MakeMaterial(const G4String& name) {
    auto* nist = G4NistManager::Instance();

    if (name == "none" || name.empty()) return nullptr;

    if (name == "TiO2_anatase") {
        // 锐钛矿 TiO2，密度 3.78 g/cm³
        if (auto* m = G4Material::GetMaterial("TiO2_anatase", false)) return m;
        G4Element* Ti = nist->FindOrBuildElement("Ti");
        G4Element* O  = nist->FindOrBuildElement("O");
        auto* m = new G4Material("TiO2_anatase", 3.78 * g/cm3, 2);
        m->AddElement(Ti, 1);
        m->AddElement(O, 2);
        return m;
    }
    if (name == "ZnO") {
        if (auto* m = G4Material::GetMaterial("ZnO", false)) return m;
        G4Element* Zn = nist->FindOrBuildElement("Zn");
        G4Element* O  = nist->FindOrBuildElement("O");
        auto* m = new G4Material("ZnO", 5.61 * g/cm3, 2);
        m->AddElement(Zn, 1);
        m->AddElement(O, 1);
        return m;
    }
    if (name == "acetonitrile") {
        // 乙腈 CH3CN (C2H3N)，密度 0.786 g/cm³（电解液近似）
        if (auto* m = G4Material::GetMaterial("acetonitrile", false)) return m;
        G4Element* C = nist->FindOrBuildElement("C");
        G4Element* H = nist->FindOrBuildElement("H");
        G4Element* N = nist->FindOrBuildElement("N");
        auto* m = new G4Material("acetonitrile", 0.786 * g/cm3, 3);
        m->AddElement(C, 2);
        m->AddElement(H, 3);
        m->AddElement(N, 1);
        return m;
    }

    // NIST 别名映射
    G4String nistName = name;
    if      (name == "Ti")  nistName = "G4_Ti";
    else if (name == "Ni")  nistName = "G4_Ni";
    else if (name == "Si")  nistName = "G4_Si";
    else if (name == "SiC") nistName = "G4_SILICON_CARBIDE";
    else if (name == "vacuum") nistName = "G4_Galactic";

    G4Material* m = nist->FindOrBuildMaterial(nistName);
    if (!m) {
        G4Exception("DetectorConstruction::MakeMaterial", "BadMaterial",
                    FatalException, ("Unknown material: " + name).c_str());
    }
    return m;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    auto* nist = G4NistManager::Instance();
    G4Material* vac = nist->FindOrBuildMaterial("G4_Galactic");

    // —— World（横向 = 容纳半宽 + 余量；纵向覆盖源与衬底）—— //
    const G4double wx = (fLateralHalfUm + 1.0) * um;
    const G4double wy = wx;
    const G4double wz = (fHeightUm + 5.0) * um;   // 覆盖 源(+0.2μm) 与 衬底(-H-1μm) + 余量
    auto* worldBox = new G4Box("World", wx, wy, wz);
    auto* worldLV  = new G4LogicalVolume(worldBox, vac, "World");
    auto* worldPV  = new G4PVPlacement(nullptr, {}, worldLV, "World",
                                       nullptr, false, 0, true);
    worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    // —— ⁶³Ni 源板：1000×1000×fSourceThick nm，底面贴 z=0 —— //
    G4Material* niMat = nist->FindOrBuildMaterial("G4_Ni");
    const G4double srcHalfXY = 500.0 * nm;
    const G4double srcHalfZ  = 0.5 * fSourceThickNm * nm;
    auto* srcBox = new G4Box("source", srcHalfXY, srcHalfXY, srcHalfZ);
    auto* srcLV  = new G4LogicalVolume(srcBox, niMat, "source");
    new G4PVPlacement(nullptr, {0, 0, srcHalfZ}, srcLV, "source",
                      worldLV, false, 0, true);
    srcLV->SetVisAttributes(new G4VisAttributes(G4Colour(0.6, 0.6, 0.6)));

    // —— Ni 阴极反射层(源上方厚 Ni)：反射结构背散射的 β 回结构 —— //
    // 真实器件中 ⁶³Ni/Ni 是宏观厚片;薄源不足以反射，故显式加厚 Ni 阴极重现低背散射损失。
    const G4double cathodeHalfZ = 1.5 * um;
    auto* cathBox = new G4Box("cathode", wx * 0.95, wy * 0.95, cathodeHalfZ);
    auto* cathLV  = new G4LogicalVolume(cathBox, niMat, "cathode");
    new G4PVPlacement(nullptr, {0, 0, 2.0 * srcHalfZ + cathodeHalfZ}, cathLV,
                      "cathode", worldLV, false, 0, true);
    cathLV->SetVisAttributes(new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)));

    // —— 结构 —— //
    switch (fStructure) {
        case Structure::Solid:         BuildSolid(worldLV);       break;
        case Structure::NanotubeArray: BuildArray(worldLV, true); break;
        case Structure::NanowireArray: BuildArray(worldLV, false);break;
    }

    // —— 可选衬底（结构底面下方）——
    if (fSubstrateName != "none" && !fSubstrateName.empty()) {
        G4Material* sub = MakeMaterial(fSubstrateName);
        const G4double subHalfZ = 0.5 * um;
        auto* subBox = new G4Box("substrate", wx * 0.9, wy * 0.9, subHalfZ);
        auto* subLV  = new G4LogicalVolume(subBox, sub, "substrate");
        new G4PVPlacement(nullptr, {0, 0, -fHeightUm * um - subHalfZ},
                          subLV, "substrate", worldLV, false, 0, true);
        subLV->SetVisAttributes(new G4VisAttributes(G4Colour(0.3, 0.3, 0.8)));
    }

    return worldPV;
}

void DetectorConstruction::BuildSolid(G4LogicalVolume* world) {
    G4Material* mat = MakeMaterial(fMaterialName);
    const G4double H = fHeightUm * um;
    const G4double halfXY = fLateralHalfUm * um;   // 横向放大以容纳侧向散射
    auto* box = new G4Box("structure", halfXY, halfXY, 0.5 * H);
    auto* lv  = new G4LogicalVolume(box, mat, "structure");
    new G4PVPlacement(nullptr, {0, 0, -0.5 * H}, lv, "structure",
                      world, false, 0, true);
    lv->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.3)));
}

void DetectorConstruction::BuildArray(G4LogicalVolume* world, bool hollow) {
    G4Material* wallMat = MakeMaterial(fMaterialName);
    G4Material* fillMat = MakeMaterial(fFillName); // 可能为 nullptr

    const G4double H    = fHeightUm * um;
    const G4double rout = fTubeOuterNm * nm;
    const G4double rin  = hollow ? (fTubeInnerNm * nm) : 0.0;
    const G4double pitch = fPitchNm * nm;
    // 阵列横向自动扩大到覆盖 ±fLateralHalfUm(容纳侧向散射)；不少于用户给的 N。
    const G4int Ncontain = static_cast<G4int>(std::ceil(2.0 * fLateralHalfUm * um / pitch));
    const G4int N = std::max(fArrayN, Ncontain);
    G4cout << "[Detector] array " << N << "x" << N << " tubes (user asked "
           << fArrayN << ", expanded for lateral containment)" << G4endl;

    // 管壁:论文真实几何为锥形管腔——内径自顶部 rin 线性收缩到底部 0(底部为实心 TiO2 锥)。
    // G4Cons 端面约定:(pRmin1,pRmax1)在 -z(底, z=-H),(pRmin2,pRmax2)在 +z(顶, z=0)。
    // 纳米线(hollow=false)则为实心圆柱 G4Tubs。
    G4VSolid* tubeSolid;
    if (hollow) {
        tubeSolid = new G4Cons("structure",
                               0.0,  rout,    // 底: 实心(rmin=0) .. rout
                               rin,  rout,    // 顶: 环形(rmin=rin) .. rout
                               0.5 * H, 0, twopi);
    } else {
        tubeSolid = new G4Tubs("structure", 0.0, rout, 0.5 * H, 0, twopi);
    }
    auto* tubeLV = new G4LogicalVolume(tubeSolid, wallMat, "structure");
    tubeLV->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.4)));

    // 管腔填充(电解液):锥形,填满管的空心部分——底部为点(0),顶部半径 rin。
    G4LogicalVolume* fillLV = nullptr;
    if (hollow && fillMat && rin > 0.0) {
        auto* fillSolid = new G4Cons("fill",
                                     0.0, 0.0,   // 底: 锥尖(0)
                                     0.0, rin,   // 顶: 半径 rin 的圆盘
                                     0.5 * H, 0, twopi);
        fillLV = new G4LogicalVolume(fillSolid, fillMat, "fill");
        fillLV->SetVisAttributes(new G4VisAttributes(G4Colour(0.2, 0.6, 1.0, 0.3)));
    }

    // 六方密堆(更贴近真实阳极氧化 TiO2 纳米管形貌):奇数行 x 偏移半个 pitch,
    // 行间距 = pitch·√3/2。缝隙率 ~9%(正方排列为 ~21%),减少 β 经缝隙穿透漏失。
    const G4double dy = pitch * std::sqrt(3.0) / 2.0;          // 行间距
    const G4int    Nrow = static_cast<G4int>(std::ceil(N * pitch / dy)); // 保持覆盖范围
    const G4double xoff = 0.5 * (N - 1) * pitch;
    const G4double yoff = 0.5 * (Nrow - 1) * dy;
    G4int copy = 0;
    for (G4int jr = 0; jr < Nrow; ++jr) {
        const G4double y = jr * dy - yoff;
        const G4double xshift = (jr % 2) ? 0.5 * pitch : 0.0;  // 奇数行错位半格
        for (G4int i = 0; i < N; ++i) {
            const G4double x = i * pitch - xoff + xshift;
            const G4ThreeVector pos(x, y, -0.5 * H);
            new G4PVPlacement(nullptr, pos, tubeLV, "structure",
                              world, false, copy, false);
            if (fillLV) {
                new G4PVPlacement(nullptr, pos, fillLV, "fill",
                                  world, false, copy, false);
            }
            ++copy;
        }
    }
}
