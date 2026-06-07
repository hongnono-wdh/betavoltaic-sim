// DetectorMessenger.cc
#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* det) : fDet(det) {
    fDir = new G4UIdirectory("/det/");
    fDir->SetGuidance("β 能量沉积仿真：几何/材料参数");

    fStructureCmd = new G4UIcmdWithAString("/det/structure", this);
    fStructureCmd->SetGuidance("结构类型: solid | nanotube_array | nanowire_array");
    fStructureCmd->SetParameterName("type", false);
    fStructureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fMaterialCmd = new G4UIcmdWithAString("/det/material", this);
    fMaterialCmd->SetGuidance("结构材料: TiO2_anatase | ZnO | Si | SiC | ...");
    fMaterialCmd->SetParameterName("mat", false);
    fMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fFillCmd = new G4UIcmdWithAString("/det/fill", this);
    fFillCmd->SetGuidance("管腔填充: none | acetonitrile");
    fFillCmd->SetParameterName("fill", false);
    fFillCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSubstrateCmd = new G4UIcmdWithAString("/det/substrate", this);
    fSubstrateCmd->SetGuidance("衬底: none | Ti | Si | ...");
    fSubstrateCmd->SetParameterName("sub", false);
    fSubstrateCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fHeightCmd = new G4UIcmdWithADouble("/det/heightUm", this);
    fHeightCmd->SetGuidance("结构高度 (um)");
    fHeightCmd->SetParameterName("h", false);
    fHeightCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fTubeOuterCmd = new G4UIcmdWithADouble("/det/tubeOuterNm", this);
    fTubeOuterCmd->SetGuidance("管外半径 (nm)");
    fTubeOuterCmd->SetParameterName("ro", false);
    fTubeOuterCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fTubeInnerCmd = new G4UIcmdWithADouble("/det/tubeInnerNm", this);
    fTubeInnerCmd->SetGuidance("管内半径 (nm)");
    fTubeInnerCmd->SetParameterName("ri", false);
    fTubeInnerCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fPitchCmd = new G4UIcmdWithADouble("/det/pitchNm", this);
    fPitchCmd->SetGuidance("阵列间距 (nm)");
    fPitchCmd->SetParameterName("p", false);
    fPitchCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fArrayNCmd = new G4UIcmdWithAnInteger("/det/arrayN", this);
    fArrayNCmd->SetGuidance("阵列边长 N（共 N×N 根）");
    fArrayNCmd->SetParameterName("n", false);
    fArrayNCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fLateralCmd = new G4UIcmdWithADouble("/det/lateralHalfUm", this);
    fLateralCmd->SetGuidance("横向半宽 (um);须 >> 电子射程以避免侧向逃逸(校准用大值)");
    fLateralCmd->SetParameterName("L", false);
    fLateralCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorMessenger::~DetectorMessenger() {
    delete fStructureCmd; delete fMaterialCmd; delete fFillCmd;
    delete fSubstrateCmd; delete fHeightCmd; delete fTubeOuterCmd;
    delete fTubeInnerCmd; delete fPitchCmd; delete fArrayNCmd; delete fLateralCmd;
    delete fDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String val) {
    if      (cmd == fStructureCmd) fDet->SetStructure(val);
    else if (cmd == fMaterialCmd)  fDet->SetMaterial(val);
    else if (cmd == fFillCmd)      fDet->SetFill(val);
    else if (cmd == fSubstrateCmd) fDet->SetSubstrate(val);
    else if (cmd == fHeightCmd)    fDet->SetHeightUm(fHeightCmd->GetNewDoubleValue(val));
    else if (cmd == fTubeOuterCmd) fDet->SetTubeOuterNm(fTubeOuterCmd->GetNewDoubleValue(val));
    else if (cmd == fTubeInnerCmd) fDet->SetTubeInnerNm(fTubeInnerCmd->GetNewDoubleValue(val));
    else if (cmd == fPitchCmd)     fDet->SetPitchNm(fPitchCmd->GetNewDoubleValue(val));
    else if (cmd == fArrayNCmd)    fDet->SetArrayN(fArrayNCmd->GetNewIntValue(val));
    else if (cmd == fLateralCmd)   fDet->SetLateralHalfUm(fLateralCmd->GetNewDoubleValue(val));
}
